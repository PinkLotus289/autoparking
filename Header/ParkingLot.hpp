#ifndef PARKINGLOT_HPP
#define PARKINGLOT_HPP

#include <string>
#include <vector>
#include <memory>
#include <format>
#include "Vehicle.hpp"
#include "ParkingSpot.hpp"
#include "DatabaseManager.hpp"

class ParkingLot {
public:
    explicit ParkingLot(const std::string& name, DatabaseManager& dbManager);

    void addVehicle(std::shared_ptr<Vehicle> vehicle);
    void removeVehicle(std::string_view licensePlate);
    std::shared_ptr<Vehicle> getVehicle(std::string_view licensePlate);

    void addParkingSpot(std::shared_ptr<ParkingSpot> spot, DatabaseManager& dbManager);
    void removeParkingSpot(int spotNumber);
    ParkingSpot* getParkingSpot(int spotNumber);

    const std::vector<std::shared_ptr<Vehicle> >& getVehicles() const;  
    const std::vector<std::shared_ptr<ParkingSpot> >& getSpots() const;
    friend double calculateFreeSpotPercentage(const ParkingLot& lot);

    void assignVehicleToSpot(const std::string& licensePlate, int spotNumber);
    void releaseParkingSpot(int spotNumber);

    void displayParkingLot(bool isAdmin) const;

    ParkingLot& operator+=(std::shared_ptr<ParkingSpot> spot);
    ParkingLot& operator-=(int spotNumber);

    void loadVehiclesFromDatabase();
    void loadParkingSpotsFromDatabase();

    void sortAndSaveSpotsToDatabase();
    DatabaseManager& getDatabaseManager() { return dbManager; }

private:
    void createTables();

    std::string name;
    std::vector<std::shared_ptr<Vehicle> > vehicles;
    std::vector<std::shared_ptr<ParkingSpot> > spots;
    DatabaseManager& dbManager;
};

template <typename T, typename Compare>
void sortObjects(std::vector<std::shared_ptr<T>>& objects, Compare comp) {
    std::sort(objects.begin(), objects.end(), [&](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
        return comp(*a, *b);
    });
}

#endif
