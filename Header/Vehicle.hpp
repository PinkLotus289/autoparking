#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <string>
#include <iostream>

class Vehicle {
public:
    virtual ~Vehicle() = default;

    virtual std::string getModel() const = 0;
    virtual std::string getLicensePlate() const = 0;
    virtual bool getStatus() const = 0; 

    virtual void display() const = 0;

    virtual void setParked(bool parked) = 0;
};

#endif
