#ifndef PARKINGSPOT_HPP
#define PARKINGSPOT_HPP

#include <memory>
#include <string>
#include "Vehicle.hpp"
#include <optional>

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

    void setParkingTime(const std::optional<std::string>& time);
    std::optional<std::string> getParkingTime() const;
    void clearParkingTime();

private:
    int number;
    std::string size;
    bool occupied;
    std::shared_ptr<Vehicle> vehicle;
    std::optional<std::string> parkingTime;
};

#endif


