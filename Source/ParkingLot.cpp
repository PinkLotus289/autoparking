#include "../Header/ParkingLot.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip>

ParkingLot::ParkingLot(std::string_view n) : name(n) {}

void ParkingLot::addCar(std::unique_ptr<Car> car) {
    cars.push_back(std::move(car));
}

void ParkingLot::removeCar(std::string_view licensePlate) {
    auto it = std::remove_if(cars.begin(), cars.end(), [&](const auto& car) {
        return car->getLicensePlate() == licensePlate;
    });
    if (it != cars.end()) {
        cars.erase(it, cars.end());
        std::cout << "Машина с номерным знаком " << licensePlate << " удалена." << std::endl;
    } else {
        std::cout << "Машина не найдена." << std::endl;
    }
}

Car* ParkingLot::getCar(std::string_view licensePlate) {
    for (const auto& car : cars) {
        if (car->getLicensePlate() == licensePlate) {
            return car.get();
        }
    }
    return nullptr;
}

void ParkingLot::addParkingSpot(std::unique_ptr<ParkingSpot> spot) {
    spots.push_back(std::move(spot));
}

void ParkingLot::removeParkingSpot(int spotNumber) {
    auto it = std::remove_if(spots.begin(), spots.end(), [&](const auto& spot) {
        return spot->getNumber() == spotNumber;
    });
    if (it != spots.end()) {
        spots.erase(it, spots.end());
        std::cout << "Парковочное место " << spotNumber << " удалено." << std::endl;
    } else {
        std::cout << "Парковочное место не найдено." << std::endl;
    }
}

ParkingSpot* ParkingLot::getParkingSpot(int spotNumber) {
    for (const auto& spot : spots) {
        if (spot->getNumber() == spotNumber) {
            return spot.get();
        }
    }
    return nullptr;
}

void ParkingLot::displayParkingLot(bool isAdmin) const {
    std::cout << "Парковка: " << name << std::endl;
    
    // Вывод таблицы для машин
    std::cout << "Машины на парковке:" << std::endl;
    std::cout << std::left 
              << std::setw(25) << "Марка"
              << std::setw(29) << "Номерной знак" 
              << std::setw(30) << "Статус" 
              << std::endl;
    std::cout << std::string(90, '-') << std::endl;

    for (const auto& car : cars) {
        car->displayCar();
    }

    // Вывод таблицы для парковочных мест
    std::cout << "\nПарковочные места:" << std::endl;
    std::cout << std::left 
              << std::setw(25) << "Номер"
              << std::setw(24) << "Размер"
              << std::setw(25) << "Занятость";
    if (isAdmin) {
        std::cout << std::setw(25) << "Информация"; // Добавлен пробел перед текстом
    }
    std::cout << std::endl;
    std::cout << std::string(90, '-') << std::endl;

    for (const auto& spot : spots) {
        spot->displayParkingSpot(isAdmin);
    }
}
