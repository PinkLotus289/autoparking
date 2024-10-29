#ifndef CAR_HPP
#define CAR_HPP

#include <string>
#include <iostream>

class Car {
public:
    Car(const std::string& model, const std::string& licensePlate);

    std::string getModel() const;
    std::string getLicensePlate() const;
    void setParked(bool parked); 
    void displayCar() const;
    bool isParked() const; 

private:
    std::string model;
    std::string licensePlate;
    bool parked = false;
};

#endif
