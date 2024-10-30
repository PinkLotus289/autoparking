#include "../Header/ParkingSpot.hpp"
#include <iostream>
#include <iomanip>
#include <format>

ParkingSpot::ParkingSpot(int num, const std::string& sz, bool isOccupied)
    : number(num), size(sz), occupied(isOccupied) {}

int ParkingSpot::getNumber() const { return number; }
std::string ParkingSpot::getSize() const { return size; }
bool ParkingSpot::isOccupied() const { return occupied; }

void ParkingSpot::setOccupied(bool isOccupied) {
    occupied = isOccupied;
    if (!isOccupied) {
        car = nullptr;
    }
}

void ParkingSpot::assignCar(std::shared_ptr<Car> newCar) {
    car = newCar;  
    occupied = true;
}

void ParkingSpot::removeCar() {
    car = nullptr;
    occupied = false;
}

std::shared_ptr<Car> ParkingSpot::getCar() const {
    return car;
}

void ParkingSpot::displayParkingSpot() const {
    std::cout << std::format("{:<10}{:<30}{:<30}", number, size, occupied ? "Занято" : "Свободно");

    if (occupied && car) {
        std::cout << std::format("{:<30}\n", car->getLicensePlate());
    } else {
        std::cout << std::format("{:<30}\n", "Свободно");
    }
}

bool ParkingSpot::operator==(const ParkingSpot& other) const {
    return this->size == other.size;
}
