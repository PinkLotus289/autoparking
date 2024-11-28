#ifndef CAR_HPP
#define CAR_HPP

#include "Vehicle.hpp"
#include <string>
#include <iostream>

class Car : public Vehicle {
public:
    Car(const std::string& model, const std::string& licensePlate);

    std::string getModel() const override;
    std::string getLicensePlate() const override;

    void setParked(bool parked) override;

    bool getStatus() const override;

    void display() const override;

    

private:
    std::string model;
    std::string licensePlate;
    bool parked = false;
};

#endif
