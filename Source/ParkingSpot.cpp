#include "../Header/ParkingSpot.hpp"
#include <iostream>
#include <iomanip>

ParkingSpot::ParkingSpot(int num, std::string_view sz, bool isOccupied)
    : number(num), size(sz), occupied(isOccupied) {}

int ParkingSpot::getNumber() const {
    return number;
}


void ParkingSpot::displayParkingSpot(bool isAdmin) const {
    std::cout << std::left 
              << std::setw(20) << number 
              << std::setw(25) << size 
              << std::setw(25) << (occupied ? "   Занято" : "Свободно");
    
    if (isAdmin) {
        std::cout << std::setw(25) << "Информация для админа"; // Добавлен пробел перед текстом
    }
    std::cout << std::endl;
}