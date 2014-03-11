#ifndef EMOKITEEGPROVIDER_H
#define EMOKITEEGPROVIDER_H

#include "eegdataprovider.h"

class EmokitEegProvider : public EegDataProvider
{
public:
    EmokitEegProvider();
signals:
    const EegMaintenanceFrame& eegMaintenanceFrame() const;
    const EegFrame& eegFrame() const;
};

#endif // EMOKITEEGPROVIDER_H
