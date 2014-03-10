#include "emokiteegprovider.h"


/**
    Battery divide by 540. 0.8-1 means GOOD
**/

EmokitEegProvider::EmokitEegProvider()
{

}


EegFrame const& EmokitEegProvider::eegFrame() const{
    return NULL;
}

EegMaintenanceFrame const& EmokitEegProvider::eegMaintenanceFrame(){
    return NULL;
}
