
#ifndef FILTERCRITERIA_HPP
#define FILTERCRITERIA_HPP

#include <QString>

struct FilterCriteria {
    QString type;                
    bool isOccupied = false;     
    QString size;                
    int numberFrom = 0;          
    int numberTo = 10000;        
    bool includeFreeVehicles = false; 
    bool includeParkedVehicles = false; 
};

#endif 
