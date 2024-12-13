#ifndef FILTERCRITERIA_HPP
#define FILTERCRITERIA_HPP

#include <QString>

struct FilterCriteria {
    QString type; 
    
    bool includeFreeVehicles = false; 
    bool includeParkedVehicles = false;

    bool includeFreeSpots = false;    
    bool includeOccupiedSpots = false;
    QString size; 
    int numberFrom = 0; 
    int numberTo = 10000; 
};

#endif 
