#include "Car.h"
#include <iostream>
#include <cstring>

Car::Car(const char* plate, const char* mod) {
    licensePlate = new char[strlen(plate) + 1];
    strcpy(licensePlate, plate);

    model = new char[strlen(mod) + 1];
    strcpy(model, mod);
}

Car::~Car() {
    delete[] licensePlate;
    delete[] model;
}

const char* Car::getLicensePlate() const {
    return licensePlate;
}

const char* Car::getModel() const {
    return model;
}

void Car::setLicensePlate(const char* plate) {
    delete[] licensePlate;
    licensePlate = new char[strlen(plate) + 1];
    strcpy(licensePlate, plate);
}

void Car::setModel(const char* mod) {
    delete[] model;
    model = new char[strlen(mod) + 1];
    strcpy(model, mod);
}

void Car::displayCarInfo() const {
    std::cout << "Car License Plate: " << licensePlate << ", Model: " << model << std::endl;
}
