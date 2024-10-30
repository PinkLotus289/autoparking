#include "../Header/ParkingLot.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <ranges>
#include <format>

ParkingLot::ParkingLot(std::string_view n, DatabaseManager& dbMgr) 
    : name(n), dbManager(dbMgr) {
    createTables(); // Создаем таблицы при инициализации
    loadCarsFromDatabase(); // Загружаем машины из базы данных
    loadParkingSpotsFromDatabase(); // Загружаем парковочные места из базы данных
}


// Функция для получения номера машины по ID машины из базы данных
static std::string getLicensePlateById(const DatabaseManager& dbManager, int carId) {
    std::string licensePlate;
    std::string carQuery = std::format("SELECT licensePlate FROM Cars WHERE id = {};", carId);
    sqlite3_stmt* carStmt;

    if (sqlite3_prepare_v2(dbManager.getDB(), carQuery.c_str(), -1, &carStmt, nullptr) == SQLITE_OK &&
        sqlite3_step(carStmt) == SQLITE_ROW) {
        licensePlate = reinterpret_cast<const char*>(sqlite3_column_text(carStmt, 0));
    }
    
    sqlite3_finalize(carStmt);
    return licensePlate;
}

std::shared_ptr<ParkingSpot> loadParkingSpot(sqlite3_stmt* stmt, ParkingLot& parkingLot, const DatabaseManager& dbManager) {
    int number = sqlite3_column_int(stmt, 0);
    std::string size = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    bool occupied = sqlite3_column_int(stmt, 2);
    int carId = sqlite3_column_int(stmt, 3);

    auto spot = std::make_shared<ParkingSpot>(number, size, occupied);

    if (occupied) {
        std::string licensePlate = getLicensePlateById(dbManager, carId);
        auto car = parkingLot.getCar(licensePlate);  // Доступ к методу через объект parkingLot
        if (car) {
            spot->assignCar(car);
        }
    }

    return spot;
}


void ParkingLot::loadCarsFromDatabase() {
    cars.clear();  // Очищаем вектор перед загрузкой данных

    std::string query = "SELECT model, licensePlate, parked FROM Cars;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dbManager.getDB(), query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string model = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string licensePlate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            bool parked = sqlite3_column_int(stmt, 2);

            auto car = std::make_shared<Car>(model, licensePlate);
            car->setParked(parked);
            cars.push_back(car);  // Добавляем в память только один раз
        }
    }
    sqlite3_finalize(stmt);
}

void ParkingLot::loadParkingSpotsFromDatabase() {
    spots.clear();
    std::string query = "SELECT number, size, occupied, carId FROM ParkingSpots;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dbManager.getDB(), query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            auto spot = loadParkingSpot(stmt, *this, dbManager);
            spots.push_back(spot);
        }
    }
    sqlite3_finalize(stmt);
}


void ParkingLot::createTables() {
    std::string createCarsTable = R"(
        CREATE TABLE IF NOT EXISTS Cars (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            model TEXT,
            licensePlate TEXT UNIQUE,
            parked INTEGER
        );
    )";
    std::string createSpotsTable = R"(
        CREATE TABLE IF NOT EXISTS ParkingSpots (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            number INTEGER UNIQUE,
            size TEXT,
            occupied INTEGER,
            carId INTEGER,
            FOREIGN KEY(carId) REFERENCES Cars(id)
        );
    )";
    dbManager.executeQuery(createCarsTable);
    dbManager.executeQuery(createSpotsTable);
}

void ParkingLot::addCar(std::shared_ptr<Car> car) {
    // Проверяем, существует ли машина с таким номером в базе данных
    std::string checkQuery = std::format("SELECT COUNT(*) FROM Cars WHERE licensePlate = '{}';", car->getLicensePlate());
    sqlite3_stmt* stmt;
    int count = 0;

    if (sqlite3_prepare_v2(dbManager.getDB(), checkQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK &&
        sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    // Если машина не существует, добавляем её в базу данных
    if (count == 0) {
        std::string query = std::format("INSERT INTO Cars (model, licensePlate, parked) VALUES ('{}', '{}', {});",
                                        car->getModel(), car->getLicensePlate(), car->isParked() ? 1 : 0);
        dbManager.executeQuery(query);
        cars.push_back(car);
    } else {
        std::cout << "Машина с номером " << car->getLicensePlate() << " уже существует.\n";
    }
}

void ParkingLot::removeCar(std::string_view licensePlate) {
    std::string query = "DELETE FROM Cars WHERE licensePlate = '" + std::string(licensePlate) + "';";
    if (dbManager.executeQuery(query)) {
        auto range = std::ranges::remove_if(cars, [&](const std::shared_ptr<Car>& car) {
            return car->getLicensePlate() == licensePlate;
        });
        cars.erase(range.begin(), range.end());
        std::cout << "Машина с номерным знаком " << licensePlate << " удалена." << std::endl;
    } else {
        std::cout << "Машина не найдена." << std::endl;
    }
}

std::shared_ptr<Car> ParkingLot::getCar(std::string_view licensePlate) {
    // Запрос к базе данных и создание объекта Car
    std::string query = "SELECT model, parked FROM Cars WHERE licensePlate = '" + std::string(licensePlate) + "';";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(dbManager.getDB(), query.c_str(), -1, &stmt, nullptr);

    std::shared_ptr<Car> car = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string model = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        bool parked = sqlite3_column_int(stmt, 1);
        car = std::make_shared<Car>(std::string(model), std::string(licensePlate)); // Используем std::string
        car->setParked(parked);
    }
    sqlite3_finalize(stmt);
    return car;
}

void ParkingLot::addParkingSpot(std::shared_ptr<ParkingSpot> spot, DatabaseManager& databaseManager) {
    // Проверяем, существует ли парковочное место с таким номером в базе данных
    std::string checkQuery = std::format("SELECT COUNT(*) FROM ParkingSpots WHERE number = {};", spot->getNumber());
    
    sqlite3_stmt* stmt;
    int count = 0;

    if (sqlite3_prepare_v2(databaseManager.getDB(), checkQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK &&
        sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    // Если парковочное место не существует, добавляем его в базу данных
    if (count == 0) {
        std::string query = std::format(
            "INSERT INTO ParkingSpots (number, size, occupied) VALUES ({}, '{}', {});",
            spot->getNumber(), spot->getSize(), spot->isOccupied() ? 1 : 0
        );
        databaseManager.executeQuery(query);
        spots.push_back(spot);
    } else {
        std::cout << "Парковочное место с номером " << spot->getNumber() << " уже существует.\n";
    }
}


void ParkingLot::removeParkingSpot(int spotNumber) {
    std::string query = std::format("DELETE FROM ParkingSpots WHERE number = {};", spotNumber);

    if (dbManager.executeQuery(query)) {
        auto range = std::ranges::remove_if(spots, [&](const std::shared_ptr<ParkingSpot>& spot) {
            return spot->getNumber() == spotNumber;
        });
        spots.erase(range.begin(), range.end());
        std::cout << "Парковочное место " << spotNumber << " удалено." << std::endl;
    } else {
        std::cout << "Парковочное место не найдено." << std::endl;
    }
}

void ParkingLot::assignCarToSpot(std::string_view licensePlate, int spotNumber) {
    std::shared_ptr<Car> car = getCar(licensePlate);  // Получаем машину по номеру
    ParkingSpot* spot = getParkingSpot(spotNumber);    // Получаем парковочное место по номеру

    if (car && spot && !spot->isOccupied()) {
        // Привязываем машину к парковочному месту
        spot->assignCar(car);
        car->setParked(true); // Обновляем статус машины в памяти

        // Обновляем статус машины в базе данных
        std::string carUpdateQuery = std::format(
            "UPDATE Cars SET parked = 1 WHERE licensePlate = '{}';", licensePlate);
        dbManager.executeQuery(carUpdateQuery);

        // Обновляем статус парковочного места в базе данных
        std::string spotUpdateQuery = std::format(
            "UPDATE ParkingSpots SET occupied = 1, carId = (SELECT id FROM Cars WHERE licensePlate = '{}') WHERE number = {};",
            licensePlate, spotNumber);
        dbManager.executeQuery(spotUpdateQuery);
    } else if (spot && spot->isOccupied()) {
        std::cout << "Парковочное место уже занято." << std::endl;
    } else {
        std::cout << "Ошибка: Машина или место не найдены." << std::endl;
    }
}


void ParkingLot::releaseParkingSpot(int spotNumber) {
    ParkingSpot* spot = getParkingSpot(spotNumber); // Ищем парковочное место

    if (spot && spot->isOccupied()) {
        if (std::shared_ptr<Car> car = spot->getCar(); car) {
            car->setParked(false); // Обновляем статус машины в памяти

            // Обновляем запись в базе данных для машины, чтобы указать, что она больше не запаркована
            std::string carUpdateQuery = std::format(
                "UPDATE Cars SET parked = 0 WHERE licensePlate = '{}';", car->getLicensePlate());
            dbManager.executeQuery(carUpdateQuery);
        }

        // Освобождаем место в памяти
        spot->removeCar();
        
        // Обновляем запись в базе данных для парковочного места
        std::string spotUpdateQuery = std::format(
            "UPDATE ParkingSpots SET occupied = 0, carId = NULL WHERE number = {};", spotNumber);
        dbManager.executeQuery(spotUpdateQuery);

        std::cout << "Парковочное место " << spotNumber << " освобождено." << std::endl;
    } else {
        std::cout << "Парковочное место либо не найдено, либо уже свободно." << std::endl;
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

const std::vector<std::shared_ptr<ParkingSpot>>& ParkingLot::getParkingSpots() const{
    return spots;
}

void ParkingLot::displayParkingLot(bool isAdmin) {
    loadCarsFromDatabase();
    loadParkingSpotsFromDatabase();

    std::cout << std::format("\nИнформация для {}:\n", isAdmin ? "администратора" : "пользователя");
    std::cout << "\n=== Информация о парковке ===\n";
    std::cout << std::format("Парковка: {}\n", name);

    if (isAdmin) {
        std::cout << "\nМашины на парковке:\n";
        std::cout << std::format("{:<15}{:<15}{:<20}\n", "Марка", "Номерной знак", "Статус");
        std::cout << std::string(50, '-') << '\n';

        for (const auto& car : cars) {
            std::cout << std::format("{:<15}{:<15}{:<20}\n", car->getModel(), car->getLicensePlate(), car->isParked() ? "Запаркован" : "Не запаркован");
        }
    }

    std::cout << "\nПарковочные места:\n";
    std::cout << std::format("{:<10}{:<15}{:<15}{:<15}\n", "Номер", "Размер", "Занятость", "Кем занято");
    std::cout << std::string(50, '-') << '\n';

    for (const auto& spot : spots) {
        std::cout << std::format("{:<10}{:<15}{:<15}", spot->getNumber(), spot->getSize(), spot->isOccupied() ? "Занято" : "Свободно");

        if (spot->isOccupied() && spot->getCar()) {
            std::cout << std::format("{:<15}\n", spot->getCar()->getLicensePlate());
        } else {
            std::cout << std::format("{:<15}\n", "-");
        }
    }

    std::cout << std::string(50, '=') << "\n\n";

    double freePercentage = calculateFreeSpotPercentage(*this);
    std::cout << std::format("Процент свободных мест: {:.2f}%\n", freePercentage);
    std::cout << std::string(50, '=') << "\n\n";
}

double calculateFreeSpotPercentage(const ParkingLot& lot) {
    if (lot.spots.empty()) {
        return 0.0;  // Если нет парковочных мест, процент свободных мест — 0%
    }

    int freeSpots = 0;
    for (const auto& spot : lot.spots) {
        if (!spot->isOccupied()) {
            freeSpots++;  // Считаем количество свободных мест
        }
    }

    // Вычисляем процент свободных мест
    return static_cast<int>(std::round((static_cast<double>(freeSpots) / lot.spots.size()) * 100));
}

ParkingLot& ParkingLot::operator+=(std::shared_ptr<ParkingSpot> spot) {
    // Используем уже существующий dbManager для добавления парковочного места в базу данных
    addParkingSpot(spot, dbManager);
    return *this;
}

ParkingLot& ParkingLot::operator-=(int spotNumber) {
    removeParkingSpot(spotNumber);  // Вызываем существующую функцию удаления
    return *this;
}
