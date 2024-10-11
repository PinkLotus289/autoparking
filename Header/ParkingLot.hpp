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

    void addCar(std::shared_ptr<Car> car);
    void removeCar(std::string_view licensePlate);
    std::shared_ptr<Car> getCar(std::string_view licensePlate);

    void addParkingSpot(std::shared_ptr<ParkingSpot> spot);
    void removeParkingSpot(int spotNumber);
    ParkingSpot* getParkingSpot(int spotNumber);

    void assignCarToSpot(std::string_view licensePlate, int spotNumber); 
    void displayParkingLot(bool isAdmin) const;

    void releaseParkingSpot(int spotNumber);

private:
    std::string name;
    std::vector<std::shared_ptr<Car>> cars;
    std::vector<std::shared_ptr<ParkingSpot>> spots;
};

#endif 
