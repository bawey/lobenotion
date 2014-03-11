#ifndef EEGDATAPROVIDER_H
#define EEGDATAPROVIDER_H

#include "eegframe.h"
#include "eegmaintenanceframe.h"
#include <QThread>
#include <QObject>

class EegDataProvider : public QThread
{
    Q_OBJECT
public:
    EegDataProvider();

    virtual const EegFrame& eegFrame() const = 0;
    virtual const EegMaintenanceFrame& eegMaintenanceFrame() = 0;
};

#endif // EEGDATAPROVIDER_H
