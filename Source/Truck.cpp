#include "../Header/Truck.hpp"
#include <iostream>
#include <format>

Truck::Truck(const std::string& cargo, const std::string& lp)
    : cargoType(cargo), licensePlate(lp){}

std::string Truck::getModel() const {
    return "Грузовик (" + cargoType + ")";
}

std::string Truck::getLicensePlate() const { return licensePlate; }

void Truck::setLoaded(bool isLoaded) {
    loaded = isLoaded;
}

bool Truck::getStatus() const { 
    return loaded; 
}

void Truck::setParked(bool parked) {
    this->loaded = parked; 
}


void Truck::display() const {
    std::cout << std::format("Truck: {:<15} {:<15} {:<20}\n", cargoType, licensePlate, loaded ? "Загружен" : "Пустой");
}
