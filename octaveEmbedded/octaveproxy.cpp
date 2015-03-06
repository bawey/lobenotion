#include "octaveproxy.h"
#include "QTextStream"
#include "iostream"
#include <QTime>
#include <QTimer>
#include <QProcess>
#include <Cell.h>
#include <QSharedPointer>
#include <QThread>
#include <settings.h>


OctaveProxy::OctaveProxy(bool redirectOutput, QObject *parent) :
    QObject(parent)
{
    string_vector octave_argv (2);
    octave_argv(0) = "embedded";
    octave_argv(1) = "-q";
    octave_main (2, octave_argv.c_str_vec(), 1);
    slotReloadScripts();

    if(redirectOutput){
        QString namedPipe = QString("/tmp/qlobesocket");

        octave_value_list redirectArgs(1);
        redirectArgs(0)=namedPipe.toStdString();

        QProcess process;
        process.start("rm", QStringList() << namedPipe);
        process.waitForFinished();

        QProcess process2;
        process2.start("mkfifo", QStringList() << namedPipe << "-m" << "777");
        process2.waitForFinished();

        outputReader = new OctaveOutputReader(namedPipe);
        connect(&outputThread, SIGNAL(started()), outputReader, SLOT(startReadingFifo()));
        outputReader->moveToThread(&outputThread);
        outputThread.start();

        feval("redirectOutput", redirectArgs);

        connect(outputReader, SIGNAL(signalFetchedOutput(QString)), this, SLOT(slotFetchedOctaveOutput(QString)));
    }
}

OctaveProxy::~OctaveProxy(){
    clean_up_and_exit(0);
}

void OctaveProxy::interpreter(){
    QTextStream stream(stdin);
    QString line;
    int parse_status;

    do {
        std::cout<<"proctave > ";
        line = stream.readLine();
        eval_string(line.toStdString(), false, parse_status);
    } while (!line.isNull());

    errorCheckEpilogue();
}

QSharedPointer<QVector<ClassifierOutput*>> OctaveProxy::askClassifier(const ClassifierInfo* modelDesc, const P3SessionInfo* sessionDesc){
    mutex.lock();
    QSharedPointer<QVector<ClassifierOutput*>> result =
        this->askClassifier(&(modelDesc->classifier), sessionDesc->getSessionPtr());
    mutex.unlock();
    return result;
}

QSharedPointer<QVector<ClassifierOutput*>> OctaveProxy::askClassifier(const ClassifierInfo *modelDesc, QList<const P3SessionInfo *>* infos){
    mutex.lock();
    octave_value testSession = mergedSession(infos);
    QSharedPointer<QVector<ClassifierOutput*>> result = this->askClassifier(&(modelDesc->classifier), &testSession);
    mutex.unlock();
    return result;
}

QSharedPointer<QVector<ClassifierOutput*>> OctaveProxy::askClassifier(const octave_value* classifier, const octave_value* session){
    QVector<ClassifierOutput*>* results = new QVector<ClassifierOutput*>();
    octave_value_list classifier_params(3);

    classifier_params(0)=*classifier;
    classifier_params(1)=*session;
    classifier_params(2)=octave_value("verbose");

    octave_value_list classified = feval("askClassifier", classifier_params);


    Matrix mx = classified(0).matrix_value();
    Matrix cnf = classified(1).matrix_value();

    for(octave_idx_type row = 0; row<mx.rows(); ++row){
        unsigned short rowNo = (unsigned short)qAbs(mx(row,0));
        unsigned short colNo = (unsigned short)qAbs(mx(row,1));
        float confidence = (float)cnf(row,0);
        results->push_back(new ClassifierOutput(rowNo, colNo, confidence));
    }
    errorCheckEpilogue();
    return QSharedPointer<QVector<ClassifierOutput*>>(results);
}

octave_value OctaveProxy::mergedSession(QList<const P3SessionInfo *> *infos){
    octave_value_list args(2);
    octave_value_list merged(1);
    merged(0)=infos->at(0)->getSession();
    if(infos->length()>1){
        for(int i=1; i<infos->length(); ++i){
            args(0)=merged(0);
            args(1)=infos->at(i)->getSession();
            merged = feval("P3SessionMerge", args);
        }
    }
    errorCheckEpilogue();
    return merged(0);
}

ClassifierInfo* OctaveProxy::pickBestModel(QList<const P3SessionInfo *> infos){
    mutex.lock();
    emit signalOctaveBusy(true);
    foreach (const P3SessionInfo* info, infos) {
       if(info->getDimStint()!=infos.at(0)->getDimStint() ||
               info->getHighlightStint()!=infos.at(0)->getHighlightStint()||
               info->getRepeats()!=infos.at(0)->getRepeats()){
           emit signalOctaveError("Training datasets' parameters mismatch!");
           return NULL;
       }
    }

    octave_value_list args(2);
    octave_value_list merged(1);
    merged(0)=infos.at(0)->getSession();
    if(infos.length()>1){
        for(int i=1; i<infos.length(); ++i){
            args(0)=merged(0);
            args(1)=infos.at(i)->getSession();
            merged = feval("P3SessionMerge", args);
        }
    }
    qDebug()<<"embarking on classifier training";

    merged(1)=octave_value(Settings::classifiersConfig().toStdString());
    merged(2)=octave_value(Settings::periodOversampling());
    merged(3)=octave_value("no");
    merged(4)=octave_value(Settings::crossValidationRounds());
    octave_value_list results = feval("pickClassifier", merged);

    std::string classifierString = feval("stringify", octave_value_list(results(1)))(0).string_value();

    QThread::currentThread()->setObjectName("testthread");

    mutex.unlock();
    if (errorCheckEpilogue()){
        emit signalOctaveBusy(false);
        return NULL;
    } else {
        ClassifierInfo* desc = new ClassifierInfo();
        /** prefill the descriptor **/
        desc->classifier=results(0);
        desc->parameters = QString::fromStdString(classifierString);
        desc->classifier = results(0);
        desc->classifierCell = results(1);
        emit signalOctaveBusy(false);
        return desc;
    }
}

P3SessionInfo* OctaveProxy::loadP3Session(QString absNameroot){
    mutex.lock();
    emit signalOctaveBusy(true);
    octave_value_list args(2);
    args(0)=octave_value("");
    args(1)=octave_value(absNameroot.toStdString());
    octave_value_list loaded = feval(P3SingleSession, args);

    loaded.append(octave_value(Settings::getDecimationFactcor()));
    octave_value_list returns = feval("downsample", loaded);

    mutex.unlock();
    if(errorCheckEpilogue()){
        emit signalOctaveBusy(false);
        return NULL;
    }else{
        emit signalOctaveBusy(false);
        return new P3SessionInfo(returns(0));
    }
}

P3SessionInfo* OctaveProxy::toP3Session(QSharedPointer<QVector<int> > signal, QSharedPointer<QVector<int> > meta, QSharedPointer<QVector<int> > targets, int channelsCount, int samplingRate, QString channelNames)
{
    mutex.lock();
    emit signalOctaveBusy(true);
    int parseStatus;
    octave_value channelsCell = eval_string(channelNames.toStdString().c_str(), true, parseStatus);

    int signalRows = signal->length()/(channelsCount+1);
    int signalCols = channelsCount+1;

    Matrix signalMx = Matrix(signalRows, signalCols);
    for(int i=0; i<signal->length(); ++i){
        signalMx(i/signalCols, i%signalCols)=((double)signal->at(i));
    }

    Matrix metaMx = Matrix(meta->length()/3, 3);
    for(int i=0; i<meta->length(); ++i){
        metaMx(i/3, i%3)=((double)meta->at(i));
    }

    Matrix trgMx = Matrix(targets->length()/3, 3);
    for(int i=0; i<targets->length(); ++i){
        trgMx(i/3, i%3)=((double)targets->at(i));
    }

    octave_value_list args=octave_value_list(6);
    args(0)=signalMx;
    args(1)=metaMx;
    args(2)=trgMx;
    args(3)=octave_value(channelsCount);
    args(4)=octave_value(samplingRate);
    args(5)=channelsCell;

    // function p3Session = P3SessionLobeRaw(signal, stimuli, targets, channelsCount, samplingRate, channelNames);
    octave_value_list loaded = feval("P3SessionLobeRaw", args);
    loaded.append(octave_value(Settings::getDecimationFactcor()));
    octave_value_list returns = feval("downsample", loaded);

    mutex.unlock();
    if (errorCheckEpilogue()){
        emit signalOctaveBusy(false);
        return NULL;
    }else{
        emit signalOctaveBusy(false);
        return new P3SessionInfo(returns(0));
    }
}

bool OctaveProxy::errorCheckEpilogue(){
    if(error_state){
        emit signalOctaveError(QString("Octave error: ").append(QString::fromStdString(last_error_message())));
        reset_error_handler();
        return true;
    }
    return false;
}

void OctaveProxy::slotReloadScripts(){
    mutex.lock();
    emit signalOctaveBusy(true);
    feval("cd", octave_value_list(Settings::octaveScriptsRoot().toStdString().c_str()));
    feval("init");
    qDebug()<<"octave scripts reloaded";
    emit signalOctaveBusy(false);
    mutex.unlock();
}

void OctaveProxy::analyzeConfidence(const ClassifierInfo * model, QSharedPointer<QList<const P3SessionInfo *>> data){
    mutex.lock();
    emit signalOctaveBusy(true);
    feval("stringify", data->at(0)->getSession());
    octave_value p3data = mergedSession(&(*data));
    octave_value_list params(3);
    params(0)=p3data;
    params(1)=octave_value(model->classifierCell);
    params(2)=octave_value(true);
    feval("getConfGaps", params);
    errorCheckEpilogue();
    qDebug()<<"Analyze Confidence ends in thread: "+(QThread::currentThread()->objectName());
    emit signalOctaveBusy(false);
    mutex.unlock();
}
