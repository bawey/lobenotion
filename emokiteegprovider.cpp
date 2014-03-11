#include "emokiteegprovider.h"


/**
    Battery divide by 540. 0.8-1 means GOOD
**/

EmokitEegProvider::EmokitEegProvider()
{

}


const EegFrame& EmokitEegProvider::eegFrame() const{
    return EegFrame();
}

const EegMaintenanceFrame& EmokitEegProvider::eegMaintenanceFrame() const{
    return EegMaintenanceFrame();
}
