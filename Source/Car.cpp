#include "../Header/Car.hpp"
#include <iostream>
#include <iomanip>


Car::Car(std::string mdl, std::string lp)
    : model(std::move(mdl)), licensePlate(std::move(lp)) {}

std::string Car::getModel() const { return model; }
std::string Car::getLicensePlate() const { return licensePlate; }

void Car::setParked(bool p) { parked = p; }

bool Car::isParked() const { 
    return parked; 
}

void Car::displayCar() const {
    std::cout << std::left << std::setw(25) << model 
              << std::left << std::setw(50) << licensePlate 
              << std::right << std::setw(30) << (parked ? "Запаркован" : "Не запаркован")
              << std::endl;
}
