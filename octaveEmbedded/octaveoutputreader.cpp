#include "octaveoutputreader.h"
#include <unistd.h>
#include <QChar>

using namespace std;

OctaveOutputReader::OctaveOutputReader(QString path, QObject *parent) :
    QObject(parent), inputPath(path)
{
}

void OctaveOutputReader::startReadingFifo(){
    qDebug()<<"void OctaveOutputReader::startReadingFifo(){";

    QDebug qdb = qDebug();

    int num, fifo;
    char temp[256];
    if ((fifo = open(this->inputPath.toStdString().c_str(), O_RDONLY)) < 0) {
        qWarning()<<strerror(errno);
        return;
    }
    QString output="";
    while (true) {
        if ((num = read(fifo, temp, sizeof(temp))) < 0) {
            qWarning()<<strerror(errno);
        }
        QString readout = QString(temp).mid(0, num);
//        if(readout.contains(QChar::LineFeed) || readout.contains(QChar::CarriageReturn)){
//            int maxIndex = qMax(readout.lastIndexOf(QChar::LineFeed), readout.lastIndexOf(QChar::CarriageReturn));
//            qDebug()<<"maxIndex@"<<maxIndex<<", char preceeding: "<<readout.at(maxIndex-1);
//            output.append(readout.mid(0, maxIndex));
//            emit signalFetchedOutput(output);
//            qDebug()<<output;
//            output.clear();
//            output.append(readout.mid(maxIndex+1));
//        }else{
//            readout.append(output);
//        }
//        readout = readout.replace(QChar::LineFeed, 'F');
//        readout = readout.replace(QChar::CarriageReturn, 'C');
//        readout = readout.replace(QChar::LineSeparator, 'S');
        qDebug()<<readout;
        emit signalFetchedOutput(readout);
    }
    close(fifo);

    qDebug("startReadingFifo() done");
}
