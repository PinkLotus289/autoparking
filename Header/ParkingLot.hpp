// ParkingLot.hpp
#ifndef PARKINGLOT_HPP
#define PARKINGLOT_HPP

#include <string>
#include <vector>
#include <memory>
#include <format>
#include "Car.hpp"
#include "ParkingSpot.hpp"
#include "DatabaseManager.hpp"


class ParkingLot{
public:
    explicit ParkingLot(const std::string& name, DatabaseManager& dbManager);

    void addCar(std::shared_ptr<Car> car);
    void removeCar(std::string_view licensePlate);
    std::shared_ptr<Car> getCar(std::string_view licensePlate);

    void addParkingSpot(std::shared_ptr<ParkingSpot> spot, DatabaseManager& dbManager);
    void removeParkingSpot(int spotNumber);
    ParkingSpot* getParkingSpot(int spotNumber);
    const std::vector<std::shared_ptr<ParkingSpot>>& getParkingSpots() const;

    void assignCarToSpot(std::string_view licensePlate, int spotNumber);
    void releaseParkingSpot(int spotNumber);

    void displayParkingLot(bool isAdmin); 

    friend double calculateFreeSpotPercentage(const ParkingLot& lot);

    ParkingLot& operator+=(std::shared_ptr<ParkingSpot> spot);
    ParkingLot& operator-=(int spotNumber);

private:
    void createTables();
    void loadCarsFromDatabase();
    void loadParkingSpotsFromDatabase();

    std::string name;
    std::vector<std::shared_ptr<Car>> cars;
    std::vector<std::shared_ptr<ParkingSpot>> spots;
    DatabaseManager& dbManager;
};

#endif 
