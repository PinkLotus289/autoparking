#ifndef TRUCK_HPP
#define TRUCK_HPP

#include "Vehicle.hpp"
#include <string>
#include <iostream>

class Truck : public Vehicle {
public:
    Truck(const std::string& cargoType, const std::string& licensePlate);

    std::string getModel() const override;
    std::string getLicensePlate() const override;

    void setLoaded(bool loaded);
    bool getStatus() const override;

    void setParked(bool parked) override;

    void display() const override;

private:
    std::string cargoType;
    std::string licensePlate;
    bool loaded = false;
};

#endif
