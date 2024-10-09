#ifndef PARKINGLOT_HPP
#define PARKINGLOT_HPP

#include <string>
#include <vector>
#include <memory>
#include "Car.hpp"
#include "ParkingSpot.hpp"

class ParkingLot {
public:
    explicit ParkingLot(std::string_view name);

    void addCar(std::unique_ptr<Car> car);
    void removeCar(std::string_view licensePlate);
    Car* getCar(std::string_view licensePlate);

    void addParkingSpot(std::unique_ptr<ParkingSpot> spot);
    void removeParkingSpot(int spotNumber);
    ParkingSpot* getParkingSpot(int spotNumber);

    void displayParkingLot(bool isAdmin) const;

private:
    std::string name;
    std::vector<std::unique_ptr<Car> > cars;
    std::vector<std::unique_ptr<ParkingSpot> > spots;
};

#endif // PARKINGLOT_HPP
