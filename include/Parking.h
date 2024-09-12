#ifndef PARKING_H
#define PARKING_H

#include "Car.h"

class Parking {
private:
    Car** cars;
    int capacity;
    int numCars;

public:
    Parking(int capacity);
    ~Parking();

    bool addCar(const char* licensePlate, const char* model);
    Car* findCar(const char* licensePlate) const;
    bool updateCar(const char* oldLicensePlate, const char* newLicensePlate, const char* newModel);
    bool removeCar(const char* licensePlate);
    int availableSpots() const;
    void listCars() const;
};

#endif // PARKING_H
