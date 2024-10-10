#ifndef CAR_HPP
#define CAR_HPP

#include <string>
#include <iostream>

class Car {
public:
    Car(std::string model, std::string licensePlate);

    std::string getModel() const;
    std::string getLicensePlate() const;
    void setParked(bool parked);  // Метод для обновления статуса запаркованности
    void displayCar() const;

private:
    std::string model;
    std::string licensePlate;
    bool parked = false;  // Инициализация переменной непосредственно в классе
};

#endif // CAR_HPP
