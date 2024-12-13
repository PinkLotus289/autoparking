#include "../Header/ParkingSpot.hpp"
#include <iostream>
#include <iomanip>
#include <format>
#include <optional>

ParkingSpot::ParkingSpot(int num, const std::string& sz, bool isOccupied)
    : number(num), size(sz), occupied(isOccupied) {}

int ParkingSpot::getNumber() const { return number; }
std::string ParkingSpot::getSize() const { return size; }

bool ParkingSpot::getStatus() const { return occupied; }

void ParkingSpot::assignVehicle(std::shared_ptr<Vehicle> newVehicle) {
    vehicle = newVehicle;
    occupied = true;
}

void ParkingSpot::removeVehicle() {
    vehicle = nullptr;
    occupied = false;
}

std::shared_ptr<Vehicle> ParkingSpot::getVehicle() const {
    return vehicle;
}

void ParkingSpot::display() const {
    if (occupied && vehicle) {
        std::cout << std::format("{:<10} {:<15} {:<15} {:<15}\n", number, size, "Занято", vehicle->getLicensePlate());
    } else {
        std::cout << std::format("{:<10} {:<15} {:<15} {:<15}\n", number, size, "Свободно", "-");
    }
}

bool ParkingSpot::operator==(const ParkingSpot& other) const {
    return this->number == other.number && this->size == other.size;
}

void ParkingSpot::setParkingTime(const std::optional<std::string>& time) {
    parkingTime = time;
}

std::optional<std::string> ParkingSpot::getParkingTime() const {
    return parkingTime;
}

void ParkingSpot::clearParkingTime() {
    parkingTime.reset();
}
