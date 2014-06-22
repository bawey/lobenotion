#ifndef METAPROCESSOR_H
#define METAPROCESSOR_H

#include <QThread>

class MetaProcessor : public QThread
{
    Q_OBJECT
public:
    explicit MetaProcessor(QObject *parent = 0);

signals:

public slots:

};

#endif // METAPROCESSOR_H
