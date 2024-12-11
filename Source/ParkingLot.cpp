#include "../Header/ParkingLot.hpp"
#include "../Header/Car.hpp"
#include "../Header/Truck.hpp"
#include <QDateTime>
#include <QString>
#include <QMessageBox>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <ranges>
#include <format>

ParkingLot::ParkingLot(const std::string& name, DatabaseManager& dbManager)
    : name(name), dbManager(dbManager) {
    createTables();
    loadVehiclesFromDatabase();
    loadParkingSpotsFromDatabase();
}

void ParkingLot::createTables() {
    std::string createVehiclesTable = R"(
        CREATE TABLE IF NOT EXISTS Vehicles (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            type TEXT,
            model TEXT,
            licensePlate TEXT UNIQUE,
            status INTEGER
        );
    )";

    std::string createSpotsTable = R"(
        CREATE TABLE IF NOT EXISTS ParkingSpots (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            number INTEGER UNIQUE,
            size TEXT,
            occupied INTEGER,
            vehicleId TEXT,
            parkingTime TEXT,
            FOREIGN KEY(vehicleId) REFERENCES Vehicles(licensePlate)
        );
    )";

    if (!dbManager.executeQuery(createVehiclesTable)) {
        std::cerr << "Ошибка при создании таблицы Vehicles." << std::endl;
    }

    if (!dbManager.executeQuery(createSpotsTable)) {
        std::cerr << "Ошибка при создании таблицы ParkingSpots." << std::endl;
    }
}


void ParkingLot::loadVehiclesFromDatabase() {
    vehicles.clear();

    std::string query = "SELECT type, model, licensePlate, status FROM Vehicles;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dbManager.getDB(), query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string model = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string licensePlate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            bool status = sqlite3_column_int(stmt, 3);

            std::shared_ptr<Vehicle> vehicle;
            if (type == "Car") {
                std::shared_ptr<Car> car = std::make_shared<Car>(model, licensePlate);
                car->setParked(status);
                vehicle = car;
            } else if (type == "Truck") {
                std::shared_ptr<Truck> truck = std::make_shared<Truck>(model, licensePlate);
                truck->setLoaded(status);
                vehicle = truck;
            }

            if (vehicle) {
                vehicles.push_back(vehicle);
            }
        }
    }
    sqlite3_finalize(stmt);
}


void ParkingLot::loadParkingSpotsFromDatabase() {
    spots.clear();

    std::string query = "SELECT number, size, occupied, vehicleId, parkingTime FROM ParkingSpots;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dbManager.getDB(), query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int number = sqlite3_column_int(stmt, 0);
            const char* sizeText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            bool occupied = sqlite3_column_int(stmt, 2);
            const char* vehicleIdText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            const char* parkingTimeText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

            std::string size = sizeText ? std::string(sizeText) : "";
            std::string vehicleId = vehicleIdText ? std::string(vehicleIdText) : "";
            std::optional<std::string> parkingTime = parkingTimeText ? std::make_optional(std::string(parkingTimeText)) : std::nullopt;

            auto spot = std::make_shared<ParkingSpot>(number, size, occupied);

            if (!vehicleId.empty()) {
                auto vehicleIt = std::find_if(vehicles.begin(), vehicles.end(),
                    [&vehicleId](const std::shared_ptr<Vehicle>& v) {
                        return v->getLicensePlate() == vehicleId;
                    });

                if (vehicleIt != vehicles.end()) {
                    spot->assignVehicle(*vehicleIt);
                }
            }

            spot->setParkingTime(parkingTime); // Устанавливаем время парковки
            spots.push_back(spot);
        }
    } else {
        std::cerr << "Ошибка SQL при загрузке парковочных мест: " << sqlite3_errmsg(dbManager.getDB()) << std::endl;
    }

    sqlite3_finalize(stmt);
}


void ParkingLot::addVehicle(std::shared_ptr<Vehicle> vehicle) {
    vehicles.push_back(vehicle);

    std::string type = std::dynamic_pointer_cast<Car>(vehicle) ? "Car" : "Truck";
    std::string query = std::format(
    "INSERT INTO Vehicles (type, model, licensePlate, status) VALUES ('{}', '{}', '{}', {});",
        type, vehicle->getModel(), vehicle->getLicensePlate(), vehicle->getStatus() ? 1 : 0);

    dbManager.executeQuery(query);
}

void ParkingLot::removeVehicle(std::string_view licensePlate) {
    // Освобождение связанного парковочного места
    auto spotIt = std::find_if(spots.begin(), spots.end(),
        [&licensePlate](const std::shared_ptr<ParkingSpot>& spot) {
            return spot->getVehicle() && spot->getVehicle()->getLicensePlate() == licensePlate;
        });

    if (spotIt != spots.end()) {
        auto& spot = *spotIt;
        spot->removeVehicle(); // Убираем привязку транспортного средства
        std::string updateSpotQuery = std::format(
            "UPDATE ParkingSpots SET occupied = 0, vehicleId = NULL, parkingTime = NULL WHERE number = {};",
            spot->getNumber()
        );

        if (!dbManager.executeQuery(updateSpotQuery)) {
            throw std::runtime_error("Не удалось обновить статус парковочного места в базе данных.");
        }
    }

    // Удаление автомобиля из памяти
    auto it = std::remove_if(vehicles.begin(), vehicles.end(),
        [&licensePlate](const std::shared_ptr<Vehicle>& v) {
            return v->getLicensePlate() == licensePlate;
        });
    vehicles.erase(it, vehicles.end());

    // Удаление автомобиля из базы данных
    std::string query = std::format("DELETE FROM Vehicles WHERE licensePlate = '{}';", licensePlate);
    if (!dbManager.executeQuery(query)) {
        throw std::runtime_error("Не удалось удалить транспортное средство из базы данных.");
    }

    std::cout << "Транспортное средство с номером " << licensePlate << " успешно удалено." << std::endl;
}


std::shared_ptr<Vehicle> ParkingLot::getVehicle(std::string_view licensePlate) {
    for (const auto& vehicle : vehicles) {
        if (vehicle->getLicensePlate() == licensePlate) {
            return vehicle;
        }
    }
    return nullptr;
}

void ParkingLot::addParkingSpot(std::shared_ptr<ParkingSpot> spot, DatabaseManager& dbManager) {

    auto existingSpot = std::find_if(spots.begin(), spots.end(),
        [&spot](const std::shared_ptr<ParkingSpot>& existing) {
            return existing->getNumber() == spot->getNumber();
        });

    if (existingSpot != spots.end()) {
        throw std::runtime_error("Парковочное место с номером " + std::to_string(spot->getNumber()) + " уже существует.");
    }

    spots.push_back(spot);

    std::string query = std::format(
        "INSERT INTO ParkingSpots (number, size, occupied) VALUES ({}, '{}', {});",
        spot->getNumber(), spot->getSize(), spot->getStatus() ? 1 : 0);

    if (!dbManager.executeQuery(query)) {
        spots.pop_back();
        throw std::runtime_error("Не удалось добавить парковочное место в базу данных.");
    }
}



void ParkingLot::removeParkingSpot(int spotNumber) {
    auto it = std::remove_if(spots.begin(), spots.end(),
        [&spotNumber](const std::shared_ptr<ParkingSpot>& s) {
            return s->getNumber() == spotNumber;
        });
    spots.erase(it, spots.end());

    std::string query = std::format("DELETE FROM ParkingSpots WHERE number = {};", spotNumber);
    dbManager.executeQuery(query);
}

const std::vector<std::shared_ptr<Vehicle>>& ParkingLot::getVehicles() const {
    return vehicles;
}

const std::vector<std::shared_ptr<ParkingSpot>>& ParkingLot::getSpots() const {
    return spots;
}

void ParkingLot::displayParkingLot(bool isAdmin) const {
    std::cout << std::format("\n=== Parking Lot: {} ===\n", name);

    if (isAdmin) {
        std::cout << "Vehicles:\n";
        for (const auto& vehicle : vehicles) {
            vehicle->display();
        }
    }

    std::cout << "\nParking Spots:\n";
    for (const auto& spot : spots) {
        spot->display();
    }
}


double calculateFreeSpotPercentage(const ParkingLot& lot) {
    if (lot.spots.empty()) {
        return 0.0; 
    }

    int freeSpots = 0;
    for (const auto& spot : lot.spots) {
        if (!spot->getStatus()) {
            freeSpots++;
        }
    }
    return (static_cast<double>(freeSpots) / lot.spots.size()) * 100;
}

ParkingLot& ParkingLot::operator+=(std::shared_ptr<ParkingSpot> spot) {
    addParkingSpot(spot, dbManager);
    return *this;
}

ParkingLot& ParkingLot::operator-=(int spotNumber) {
    removeParkingSpot(spotNumber);  
    return *this;
}

void ParkingLot::assignVehicleToSpot(const std::string& licensePlate, int spotNumber, const std::string& parkingTime) {
    auto spotIt = std::find_if(spots.begin(), spots.end(),
        [spotNumber](const std::shared_ptr<ParkingSpot>& spot) {
            return spot->getNumber() == spotNumber;
        });

    if (spotIt == spots.end()) {
        throw std::runtime_error("Парковочное место с номером " + std::to_string(spotNumber) + " не найдено.");
    }

    auto& spot = *spotIt;

    if (spot->getStatus()) {
        throw std::runtime_error("Парковочное место с номером " + std::to_string(spotNumber) + " уже занято.");
    }

    auto vehicleIt = std::find_if(vehicles.begin(), vehicles.end(),
        [&licensePlate](const std::shared_ptr<Vehicle>& vehicle) {
            return vehicle->getLicensePlate() == licensePlate;
        });

    if (vehicleIt == vehicles.end()) {
        throw std::runtime_error("Транспортное средство с номером " + licensePlate + " не найдено.");
    }

    auto& vehicle = *vehicleIt;

    spot->assignVehicle(vehicle);
    vehicle->setParked(true);

    std::string updateSpotQuery = std::format(
        "UPDATE ParkingSpots SET occupied = 1, vehicleId = '{}', parkingTime = '{}' WHERE number = {};",
        licensePlate,
        parkingTime.empty() ? QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString() : parkingTime,
        spotNumber
    );

    std::string updateVehicleQuery = std::format(
        "UPDATE Vehicles SET status = 1 WHERE licensePlate = '{}';",
        licensePlate
    );

    if (!dbManager.executeQuery(updateSpotQuery)) {
        throw std::runtime_error("Не удалось обновить парковочное место в базе данных.");
    }

    if (!dbManager.executeQuery(updateVehicleQuery)) {
        throw std::runtime_error("Не удалось обновить статус транспортного средства в базе данных.");
    }
}


void ParkingLot::releaseParkingSpot(int spotNumber) {
    
    auto spotIt = std::find_if(spots.begin(), spots.end(),
        [spotNumber](const std::shared_ptr<ParkingSpot>& spot) {
            return spot->getNumber() == spotNumber;
        });

    if (spotIt == spots.end()) {
        std::cerr << "Ошибка: Парковочное место с номером " << spotNumber << " не найдено." << std::endl;
        return;
    }

    auto& spot = *spotIt;

    if (!spot->getStatus()) {
        std::cerr << "Ошибка: Парковочное место с номером " << spotNumber << " уже свободно." << std::endl;
        return;
    }

    auto parkingTime = spot->getParkingTime();
    if (!parkingTime || parkingTime->empty()) {
        std::cerr << "Ошибка: Время начала парковки отсутствует для места " << spotNumber << std::endl;
        return;
    }

    QDateTime startTime = QDateTime::fromString(QString::fromStdString(*parkingTime), "yyyy-MM-dd HH:mm:ss");
    if (!startTime.isValid()) {
        std::cerr << "Ошибка: Неверный формат времени парковки: " << *parkingTime << std::endl;
        return;
    }

    QDateTime endTime = QDateTime::currentDateTime();

    qint64 durationInSeconds = startTime.secsTo(endTime);
    if (durationInSeconds <= 0) {
        std::cerr << "Ошибка: Время парковки отрицательное или равно нулю." << std::endl;
        return;
    }

    double durationInHours = static_cast<double>(durationInSeconds) / 3600.0; 
    //const double hourlyRate = 100.0; 
    double totalCost = durationInHours * hourlyRate;

    auto vehicle = spot->getVehicle();
    if (vehicle) {
        vehicle->setParked(false);

        std::string updateVehicleQuery = std::format(
            "UPDATE Vehicles SET status = 0 WHERE licensePlate = '{}';",
            vehicle->getLicensePlate()
        );

        if (!dbManager.executeQuery(updateVehicleQuery)) {
            std::cerr << "Ошибка: Не удалось обновить статус транспортного средства в базе данных." << std::endl;
        }
    }

    spot->removeVehicle();

    std::string updateSpotQuery = std::format(
        "UPDATE ParkingSpots SET occupied = 0, vehicleId = NULL, parkingTime = NULL WHERE number = {};",
        spotNumber
    );

    if (!dbManager.executeQuery(updateSpotQuery)) {
        std::cerr << "Ошибка: Не удалось обновить статус парковочного места в базе данных." << std::endl;
    }

    QMessageBox::information(nullptr, "Информация о парковке",
        QString("Автомобиль находился на парковке %1 часов.\nОбщая стоимость: %2 рублей.")
        .arg(durationInHours, 0, 'f', 2)
        .arg(totalCost, 0, 'f', 2));

    std::cout << "Парковочное место номер " << spotNumber << " успешно освобождено." << std::endl;
}


ParkingSpot* ParkingLot::getParkingSpot(int spotNumber) {
    for (const auto& spot : spots) {
        if (spot->getNumber() == spotNumber) {
            return spot.get();
        }
    }
    return nullptr;
}


void ParkingLot::sortAndSaveSpotsToDatabase() {
    
    sortObjects(spots, [](const ParkingSpot& a, const ParkingSpot& b) {
        return a.getNumber() < b.getNumber();
    });

    std::string deleteQuery = "DELETE FROM ParkingSpots;";
    if (!dbManager.executeQuery(deleteQuery)) {
        std::cerr << "Ошибка: Не удалось очистить таблицу ParkingSpots в базе данных." << std::endl;
        return;
    }

    for (const auto& spot : spots) {
        std::string insertQuery = std::format(
            "INSERT INTO ParkingSpots (number, size, occupied, vehicleId) VALUES ({}, '{}', {}, {});",
            spot->getNumber(),
            spot->getSize(),
            spot->getStatus() ? 1 : 0,
            spot->getVehicle() ? "'" + spot->getVehicle()->getLicensePlate() + "'" : "NULL"
        );

        if (!dbManager.executeQuery(insertQuery)) {
            std::cerr << "Ошибка: Не удалось добавить парковочное место номер " << spot->getNumber()
                      << " в базу данных." << std::endl;
        }
    }

    std::cout << "Парковочные места успешно отсортированы и загружены в базу данных." << std::endl;
}

