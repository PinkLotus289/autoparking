#include "../Header/ParkingSpot.hpp"
#include <iostream>
#include <iomanip>

ParkingSpot::ParkingSpot(int num, std::string_view sz, bool isOccupied)
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
    car = newCar;  // Присваиваем параметр полю класса
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
              << std::setw(20) << size
              << std::setw(15) << (occupied ? "Занято" : "Свободно")
              << std::setw(20);

    if (occupied && car) {
        std::cout << car->getLicensePlate();
    } else {
        std::cout << "Свободно";
    }

    std::cout << std::endl;
}
