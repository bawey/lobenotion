#include "octaveproxy.h"
#include "QTextStream"
#include "iostream"
#include <QTime>
#include <QTimer>
#include <QProcess>
#include <classifiersmodel.h>

OctaveProxy::OctaveProxy(QObject *parent) :
    QObject(parent)
{
    string_vector octave_argv (2);
    octave_argv(0) = "embedded";
    octave_argv(1) = "-q";
    octave_main (2, octave_argv.c_str_vec (), 1);
    feval("addpath", octave_value_list("scripts"));
    feval("cd", octave_value_list("scripts"));
    feval("init");

    QString serverName = QString("/tmp/qlobesocket");
    qDebug()<<"Server name: "<<serverName;

    octave_value_list redirectArgs(1);
    redirectArgs(0)=serverName.toStdString();

    QProcess process;
    process.start("rm", QStringList() << serverName);
    process.waitForFinished();

    QProcess process2;
    process2.start("mkfifo", QStringList() << serverName << "-m" << "777");
    process2.waitForFinished();

    outputReader = new OctaveOutputReader(serverName);
    connect(&outputThread, SIGNAL(started()), outputReader, SLOT(startReadingFifo()));
    outputReader->moveToThread(&outputThread);

    outputThread.start();

    feval("redirectOutput", redirectArgs);

    connect(outputReader, SIGNAL(signalFetchedOutput(QString)), this, SLOT(slotFetchedOctaveOutput(QString)));
}

OctaveProxy::~OctaveProxy(){
    clean_up_and_exit(0);
}

void OctaveProxy::demo(){
    octave_value_list first_params;
    first_params(0)=3;
    first_params(1)=7;
    octave_value_list result = feval("dummy", first_params);
    octave_value_list processed_result = feval("yummy", result);

    if (!error_state && processed_result.length () > 0){

    }else {
        std::cout << "invalid \n";
    }
}

void OctaveProxy::diag(){

}

void OctaveProxy::simpleTrainModel(QString dirpath, QString subject, QList<unsigned short> sessions){
    /**
     * NEED TO SPECIFY CLASSIFIER PARAMETERS, like: @BalancedClassifier, {@ClassifierLogReg, max_iterations, lambda} :/
     */

    int parse_status;

    octave_value fhandle = eval_string("@BalancedClassifier", false, parse_status);
    octave_value fhandle_params = eval_string("{@ClassifierLogReg, 100, 0.01}", false, parse_status);
    octave_value_list train_params(3);
    train_params(0)=fhandle;
    train_params(1)=fhandle_params;
    train_params(2)=loadMergeAndDownsample(dirpath, subject, sessions);
//    octave_value fhandle_unit = eval_string("@ClassifierLogReg");
//    Cell classifierParams = Cell(3);
//    classifierParams(0)=fhandle_unit

    octave_value_list trained = feval("trainClassifier", train_params);

    //store the model and let it be used later with further tests
    this->model=trained(0);
    this->tr_mean=trained(1);
    this->tr_std=trained(2);
}

void OctaveProxy::simpleClassifySessions(QString dirpath, QString subject, QList<unsigned short> sessions){

    octave_value_list classifier_params(4);
    classifier_params(0)=this->model;
    classifier_params(1)=loadMergeAndDownsample(dirpath, subject, sessions);
    classifier_params(2)=this->tr_mean;
    classifier_params(3)=this->tr_std;

    octave_value_list classified = feval("askClassifier", classifier_params);

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
}

octave_value OctaveProxy::loadMergeAndDownsample(QString dirpath, QString subject, QList<unsigned short> sessions){
    octave_value oct_dir = octave_value(dirpath.toStdString());

    RowVector oct_sessions(sessions.length());
    for(int i=0; i<sessions.length(); ++i){
        oct_sessions(i)=sessions.at(i);
    }

    octave_value oct_subject = octave_value(subject.toStdString());

    octave_value_list params;
    params(0)=oct_dir;
    params(1)=oct_subject;
    params(2)=oct_sessions;


    octave_value_list loaded = feval(P3MultiSession, params);
    loaded.append(octave_value(DOWNSAMPLING_RATE));

    octave_value_list downsampled = feval("downsample", loaded);
    return downsampled(0);
}

void OctaveProxy::pickBestModel(QString dirpath, QString subject, QList<unsigned short> sessions){
    octave_value session = loadMergeAndDownsample(dirpath, subject, sessions);
    octave_value_list args(1);
    args(0)=session;
    octave_value_list results = feval("pickClassifier", args);
    this->model=results(0);
    this->tr_mean=results(1);
    this->tr_std=results(2);
}

void OctaveProxy::askClassifier(const ClassifiersModel::ClassifierDescriptor *modelDesc, QList<const P3SessionInfo *> infos){
    octave_value testSession = mergedSession(infos);
    octave_value_list classifier_params(4);

    classifier_params(0)=modelDesc->classifier;
    classifier_params(1)=testSession;
    classifier_params(2)=modelDesc->tdMean;
    classifier_params(3)=modelDesc->tdStd;

    octave_value_list classified = feval("askClassifier", classifier_params);
}

octave_value OctaveProxy::mergedSession(QList<const P3SessionInfo *> infos){
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
    return merged(0);
}

octave_value OctaveProxy::pickBestModel(QList<const P3SessionInfo *> infos,
                                        ClassifiersModel::ClassifierDescriptor* desc){

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
    octave_value_list results = feval("pickClassifier", merged);

    /** prefill the descriptor **/
    desc->classifier=results(0);
    desc->tdMean=results(1);
    desc->tdStd=results(2);
    std::string classifierString = feval("stringify", octave_value_list(results(3)))(0).string_value();
    desc -> parameters = QString::fromStdString(classifierString);

    return results(0);
}

octave_value_list OctaveProxy::loadSessions(QStringList nameRoots){
    return loadSessions("", nameRoots);
}

octave_value_list OctaveProxy::loadSessions(QString dirpath, QStringList nameRoots){
    octave_value_list values(nameRoots.length());
    unsigned short i = 0;
    foreach(QString nameroot, nameRoots){
        octave_value_list args(2);
        args(0)=octave_value(dirpath.toStdString());
        args(1)=octave_value(nameroot.toStdString());
        octave_value_list returns = feval(P3SingleSession, args);
        values(i++) = returns(0);
    }
    return values;
}
