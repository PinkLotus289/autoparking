#include "../Header/Car.hpp"
#include <iostream>
#include <iomanip>
#include <format>

Car::Car(const std::string& mdl, const std::string& lp)
    : model(mdl), licensePlate(lp) {}

std::string Car::getModel() const { return model; }
std::string Car::getLicensePlate() const { return licensePlate; }

void Car::setParked(bool p) { parked = p; }

bool Car::getStatus() const { 
    return parked; 
}

void Car::display() {
    std::cout << std::format("{:<25}{:<50}{:<30}\n", model, licensePlate, parked ? "Запаркован" : "Не запаркован");
}
