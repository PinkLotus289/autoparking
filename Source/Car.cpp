#include "../Header/Car.hpp"
#include <iostream>
#include <iomanip>


Car::Car(std::string mdl, std::string lp) 
    : model(std::move(mdl)), licensePlate(std::move(lp)), parked(false) {}

std::string Car::getModel() const { return model; }
std::string Car::getLicensePlate() const { return licensePlate; }

void Car::setParked(bool p) { parked = p; }

void Car::displayCar() const {
    std::cout << std::left << std::setw(25) << model 
              << std::setw(29) << licensePlate 
              << std::setw(30) << (parked ? "Запаркован" : "Не запаркован")
              << std::endl;
}