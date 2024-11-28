#include "../Header/Car.hpp"
#include <iostream>
#include <format>

Car::Car(const std::string& mdl, const std::string& lp)
    : model(mdl), licensePlate(lp) {}
    
std::string Car::getModel() const { return model; }
std::string Car::getLicensePlate() const { return licensePlate; }

void Car::setParked(bool parked) {
    this->parked = parked;
}

bool Car::getStatus() const { 
    return parked; 
}

void Car::display() const {
    std::cout << std::format("Car: {:<15} {:<15} {:<20}\n", model, licensePlate, parked ? "Запаркован" : "Не запаркован");
}
