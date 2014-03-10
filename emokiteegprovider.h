#ifndef EMOKITEEGPROVIDER_H
#define EMOKITEEGPROVIDER_H

#include "eegdataprovider.h"

class EmokitEegProvider : public EegDataProvider
{
public:
    EmokitEegProvider();
signals:
    EegFrame const& eegMaintenanceFrame() const;
    EegMaintenanceFrame const& eegFrame() const;

};

#endif // EMOKITEEGPROVIDER_H
