#ifndef PARKINGSPOT_HPP
#define PARKINGSPOT_HPP

#include <memory>
#include <string>
#include "Vehicle.hpp"

class ParkingSpot {
public:
    ParkingSpot(int number, const std::string& size, bool occupied = false);

    int getNumber() const;
    std::string getSize() const;
    bool getStatus() const;

    void assignVehicle(std::shared_ptr<Vehicle> vehicle);
    void removeVehicle();
    std::shared_ptr<Vehicle> getVehicle() const;

    void display() const;
    bool operator==(const ParkingSpot& other) const;

private:
    int number;
    std::string size;
    bool occupied;
    std::shared_ptr<Vehicle> vehicle;
};

#endif


