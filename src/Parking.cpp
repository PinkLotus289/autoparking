#include "Parking.h"
#include <iostream>
#include <cstring>

Parking::Parking(int capacity) : capacity(capacity), numCars(0) {
    cars = new Car*[capacity];
}

Parking::~Parking() {
    for (int i = 0; i < numCars; ++i) {
        delete cars[i];
    }
    delete[] cars;
}

bool Parking::addCar(const char* licensePlate, const char* model) {
    if (numCars >= capacity) {
        std::cout << "Parking is full!" << std::endl;
        return false;
    }

    cars[numCars] = new Car(licensePlate, model);
    numCars++;
    std::cout << "Car added: " << licensePlate << std::endl;
    return true;
}

Car* Parking::findCar(const char* licensePlate) const {
    for (int i = 0; i < numCars; ++i) {
        if (strcmp(cars[i]->getLicensePlate(), licensePlate) == 0) {
            return cars[i];
        }
    }
    return nullptr;
}

bool Parking::updateCar(const char* oldLicensePlate, const char* newLicensePlate, const char* newModel) {
    Car* car = findCar(oldLicensePlate);
    if (car != nullptr) {
        car->setLicensePlate(newLicensePlate);
        car->setModel(newModel);

        std::cout << "Car updated: " << newLicensePlate << std::endl;
        return true;
    }
    std::cout << "Car not found: " << oldLicensePlate << std::endl;
    return false;
}

bool Parking::removeCar(const char* licensePlate) {
    for (int i = 0; i < numCars; ++i) {
        if (strcmp(cars[i]->getLicensePlate(), licensePlate) == 0) {
            delete cars[i];
            cars[i] = cars[numCars - 1];
            numCars--;
            std::cout << "Car removed: " << licensePlate << std::endl;
            return true;
        }
    }
    std::cout << "Car not found: " << licensePlate << std::endl;
    return false;
}

int Parking::availableSpots() const {
    return capacity - numCars;
}

void Parking::listCars() const {
    if (numCars == 0) {
        std::cout << "Parking is empty." << std::endl;
    } else {
        for (int i = 0; i < numCars; ++i) {
            cars[i]->displayCarInfo();
        }
    }
}
