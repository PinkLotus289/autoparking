#ifndef EXPORTMANAGER_HPP
#define EXPORTMANAGER_HPP

#include <QString>
#include <vector>
#include <memory>
#include "Vehicle.hpp"
#include "ParkingSpot.hpp"
#include "FilterCriteria.hpp"

class ExportManager {
public:
    
    static void exportVehicles(const QString &filePath,
                               const std::vector<std::shared_ptr<Vehicle>> &vehicles,
                               const FilterCriteria &criteria);

    
    static void exportParkingSpots(const QString &filePath,
                                   const std::vector<std::shared_ptr<ParkingSpot>> &spots,
                                   const FilterCriteria &criteria);
    
    static void exportData(const QString &filePath,
                           const std::vector<std::shared_ptr<Vehicle>> &vehicles,
                           const std::vector<std::shared_ptr<ParkingSpot>> &spots,
                           const FilterCriteria &criteria);
};

#endif 

