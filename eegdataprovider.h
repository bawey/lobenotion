#ifndef EEGDATAPROVIDER_H
#define EEGDATAPROVIDER_H

#include "eegframe.h"
#include "eegmaintenanceframe.h"
#include <QThread>
#include <QObject>

class EegDataProvider : public QObject, public QThread
{
    Q_OBJECT
public:
    EegDataProvider();
signals:
    virtual EegFrame const& eegFrame() const;
    virtual EegMaintenanceFrame const& eegMaintenanceFrame() const;
};

#endif // EEGDATAPROVIDER_H
