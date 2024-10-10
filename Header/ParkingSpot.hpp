#ifndef PARKINGSPOT_HPP
#define PARKINGSPOT_HPP

#include <string>
#include <memory>
#include "Car.hpp"

class ParkingSpot {
public:
    ParkingSpot(int number, std::string_view size, bool occupied = false);

    int getNumber() const;
    std::string getSize() const;
    bool isOccupied() const;
    
    void displayParkingSpot(bool isAdmin) const;
    void setOccupied(bool isOccupied);

    void assignCar(std::shared_ptr<Car> car);
    void removeCar();
    std::shared_ptr<Car> getCar() const;

private:
    int number;
    std::string size;
    bool occupied;
    std::shared_ptr<Car> car = nullptr;  // Инициализация переменной car как nullptr прямо в объявлении
};

#endif // PARKINGSPOT_HPP
