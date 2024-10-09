#ifndef CAR_HPP
#define CAR_HPP

#include <string>

class Car {
public:
    Car(std::string_view brand, std::string_view licensePlate, bool reserved = false);

    std::string_view getLicensePlate() const;
    void displayCar() const;

private:
    std::string brand;
    std::string licensePlate;
    bool reserved;
};

#endif // CAR_HPP

