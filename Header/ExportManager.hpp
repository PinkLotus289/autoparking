
#ifndef EXPORTMANAGER_HPP
#define EXPORTMANAGER_HPP

#include <QString>
#include <vector>
#include <memory>
#include "Vehicle.hpp"
#include "ParkingSpot.hpp"
#include "FilterCriteria.hpp" // Опишем структуру фильтров здесь

class ExportManager {
public:
    // Экспорт автомобилей
    static void exportVehicles(const QString& format, const QString& filePath, 
                               const std::vector<std::shared_ptr<Vehicle>>& vehicles, 
                               const FilterCriteria& criteria);

    // Экспорт парковочных мест
    static void exportParkingSpots(const QString& format, const QString& filePath, 
                                   const std::vector<std::shared_ptr<ParkingSpot>>& spots, 
                                   const FilterCriteria& criteria);

    void exportData(const QString &filePath, 
                    const std::vector<std::shared_ptr<Vehicle>> &vehicles, 
                    const std::vector<std::shared_ptr<ParkingSpot>> &spots);
};

#endif // EXPORTMANAGER_HPP
