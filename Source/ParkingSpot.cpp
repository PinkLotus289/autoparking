#include "../Header/ParkingSpot.hpp"
#include <iostream>
#include <iomanip>

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
    std::cout << std::left << std::setw(10) << number
              << std::left << std::setw(30) << size
              << std::left << std::setw(30) << (occupied ? "Занято" : "Свободно")
              << std::left << std::setw(30);

    if (occupied && car) {  // Если место занято и есть машина
        std::cout << std::left << car->getLicensePlate();  // Отображаем номер машины
    } else {
        std::cout << std::left << "Свободно";
    }

    std::cout << std::endl;
}

bool ParkingSpot::operator==(const ParkingSpot& other) const {
    return this->size == other.size;
}
