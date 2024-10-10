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
    std::cout << "\nИнформация для " << (isAdmin ? "администратора:" : "пользователя:") << "\n";
    std::cout << "\n=== Информация о парковке ===\n";
    std::cout << "Парковка: " << name << "\n";

    // Вывод таблицы для машин только для администратора
    if (isAdmin) {
        std::cout << "\nМашины на парковке:\n";
        std::cout << std::left << std::setw(20) << "Марка"
                  << std::left << std::setw(34) << "Номерной знак" 
                  << std::left << std::setw(30) << "Статус" 
                  << std::endl;
        std::cout << std::string(60, '-') << std::endl;

        for (const auto& car : cars) {
            std::cout << std::left 
                      << std::setw(15) << car->getModel()
                      << std::setw(25) << car->getLicensePlate()
                      << std::setw(30) << (car->isParked() ? "Запаркован" : "Не запаркован")
                      << std::endl;
        }
    }

    // Вывод таблицы для парковочных мест
    std::cout << "\nПарковочные места:\n";
    std::cout << std::left << std::setw(15) << "Номер"
              << std::left << std::setw(25) << "Размер"
              << std::left << std::setw(25) << "Занятость"
              << std::left << std::setw(20) << "Кем занято" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    for (const auto& spot : spots) {
        // Если администратор, отображаем все места, иначе только свободные
        if (isAdmin || !spot->isOccupied()) {
            std::cout << std::left << std::setw(10) << spot->getNumber()
                      << std::left << std::setw(29) << spot->getSize()
                      << std::left << std::setw(25) << (spot->isOccupied() ? "Занято" : "Свободно");
            
            if (spot->isOccupied() && spot->getCar()) {
                std::cout << std::left << std::setw(20) << spot->getCar()->getLicensePlate();
            } else {
                std::cout << std::left << std::setw(20) << "-";
            }
            std::cout << std::endl;
        }
    }

    std::cout << std::string(60, '=') << "\n" << std::endl;
}


void ParkingLot::releaseParkingSpot(int spotNumber) {
    ParkingSpot* spot = getParkingSpot(spotNumber);

    if (spot && spot->isOccupied()) {
        if (std::shared_ptr<Car> car = spot->getCar(); car) {  // Используем init-statement
            car->setParked(false);  // Устанавливаем статус как "Не запаркован"
        }
        spot->removeCar();
        std::cout << "Парковочное место " << spotNumber << " теперь свободно." << std::endl;
    } else {
        std::cout << "Парковочное место либо не найдено, либо уже свободно." << std::endl;
    }
}