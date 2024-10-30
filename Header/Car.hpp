// Car.hpp
#ifndef CAR_HPP
#define CAR_HPP

#include <string>
#include <iostream>
#include <format>
#include "TerminalDisplay.hpp"
#include "Get.hpp"

class Car : public TerminalDisplay, public GetField {
public:
    Car(const std::string& model, const std::string& licensePlate);

    std::string getModel() const;
    std::string getLicensePlate() const;

    void setParked(bool parked);
    bool getStatus() const override;

    void display() override; // Реализация виртуальной функции display

private:
    std::string model;
    std::string licensePlate;
    bool parked = false;
};

#endif // CAR_HPP
