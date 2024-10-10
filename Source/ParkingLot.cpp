#include "../Header/ParkingLot.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <ranges>

ParkingLot::ParkingLot(std::string_view n) : name(n) {}

void ParkingLot::addCar(std::shared_ptr<Car> car) {
    cars.push_back(std::move(car));
}

void ParkingLot::removeCar(std::string_view licensePlate) {
    auto range = std::ranges::remove_if(cars, [&](const std::shared_ptr<Car>& car) {
        return car->getLicensePlate() == licensePlate;
    });
    if (range.begin() != cars.end()) {
        cars.erase(range.begin(), range.end());
        std::cout << "Машина с номерным знаком " << licensePlate << " удалена." << std::endl;
    } else {
        std::cout << "Машина не найдена." << std::endl;
    }
}

std::shared_ptr<Car> ParkingLot::getCar(std::string_view licensePlate) {
    for (const auto& car : cars) {
        if (car->getLicensePlate() == licensePlate) {
            return car;
        }
    }
    return nullptr;
}

void ParkingLot::addParkingSpot(std::shared_ptr<ParkingSpot> spot) {
    spots.push_back(std::move(spot));
}

void ParkingLot::removeParkingSpot(int spotNumber) {
    auto range = std::ranges::remove_if(spots, [&](const std::shared_ptr<ParkingSpot>& spot) {
        return spot->getNumber() == spotNumber;
    });
    if (range.begin() != spots.end()) {
        spots.erase(range.begin(), range.end());
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

void ParkingLot::assignCarToSpot(std::string_view licensePlate, int spotNumber) {
    std::shared_ptr<Car> car = getCar(licensePlate);
    ParkingSpot* spot = getParkingSpot(spotNumber);

    if (car && spot && !spot->isOccupied()) {
        spot->assignCar(car);
        car->setParked(true);  // Устанавливаем статус запаркованности
        std::cout << "Машина с номером " << licensePlate << " закреплена за местом " << spotNumber << "." << std::endl;
    } else if (spot && spot->isOccupied()) {
        std::cout << "Парковочное место уже занято." << std::endl;
    } else {
        std::cout << "Ошибка: Машина или место не найдены." << std::endl;
    }
}

void ParkingLot::displayParkingLot(bool isAdmin) const {
    std::cout << "Парковка: " << name << std::endl;

    // Вывод таблицы для машин только для администратора
    if (isAdmin) {
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
    }

    // Вывод таблицы для парковочных мест
    std::cout << "\nПарковочные места:" << std::endl;
    std::cout << std::left 
              << std::setw(25) << "Номер"
              << std::setw(24) << "Размер"
              << std::setw(25) << "Занятость"
              << std::setw(20) << "Кем занято" << std::endl;
    std::cout << std::string(90, '-') << std::endl;

    for (const auto& spot : spots) {
        if (isAdmin || !spot->isOccupied()) {
            spot->displayParkingSpot(isAdmin);
        }
    }
}


void ParkingLot::releaseParkingSpot(int spotNumber) {
    ParkingSpot* spot = getParkingSpot(spotNumber);

    if (spot && spot->isOccupied()) {
        std::shared_ptr<Car> car = spot->getCar();
        if (car) {
            car->setParked(false);  // Устанавливаем статус как "Не запаркован"
        }
        spot->removeCar();
        std::cout << "Парковочное место " << spotNumber << " теперь свободно." << std::endl;
    } else {
        std::cout << "Парковочное место либо не найдено, либо уже свободно." << std::endl;
    }
}

