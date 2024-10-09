#include "../Header/Car.hpp"
#include <iostream>
#include <iomanip>

Car::Car(std::string_view b, std::string_view lp, bool isReserved)
    : brand(b), licensePlate(lp), reserved(isReserved) {}

std::string_view Car::getLicensePlate() const {
    return licensePlate;
}

void Car::displayCar() const {
    std::cout << std::left 
              << std::setw(20) << brand 
              << std::setw(20) << licensePlate 
              << std::setw(25) << (reserved ? "Зарезервировано" : "Не зарезервировано") 
              << std::endl;
}
