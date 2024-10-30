// ParkingSpot.hpp
#ifndef PARKINGSPOT_HPP
#define PARKINGSPOT_HPP

#include <string>
#include <memory>
#include <format>
#include "Car.hpp"
#include "TerminalDisplay.hpp"

class ParkingSpot : public TerminalDisplay, public GetField {
public:
    ParkingSpot(int number, const std::string& size, bool occupied = false);

    int getNumber() const;
    std::string getSize() const;
    bool getStatus() const override;

    void display() override; 

    void setOccupied(bool isOccupied);

    void assignCar(std::shared_ptr<Car> car);
    void removeCar();
    std::shared_ptr<Car> getCar() const;

    bool operator==(const ParkingSpot& other) const;

private:
    int number;
    std::string size;
    bool occupied;
    std::shared_ptr<Car> car = nullptr;
};

#endif // PARKINGSPOT_HPP

