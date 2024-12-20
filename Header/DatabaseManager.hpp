#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <sqlite3.h>
#include <string>
#include <iostream>

class DatabaseManager {
public:
    explicit DatabaseManager(const std::string& dbFile) {
        if (sqlite3_open(dbFile.c_str(), &db) != SQLITE_OK) {
            std::cerr << "Ошибка при открытии базы данных: " << sqlite3_errmsg(db) << std::endl;
        } else {
            std::cout << "Подключение к базе данных успешно." << std::endl;
        }
    }

    ~DatabaseManager() {
        if (db) {
            sqlite3_close(db);
            std::cout << "Подключение к базе данных закрыто." << std::endl;
        }
    }

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    // Разрешаем перемещение
    DatabaseManager(DatabaseManager&& other) noexcept : db(other.db) {
        other.db = nullptr;  
    }

    DatabaseManager& operator=(DatabaseManager&& other) noexcept {
        if (this != &other) {
            if (db) {
                sqlite3_close(db);
            }
            db = other.db;      
            other.db = nullptr; 
        }
        return *this;
    }

    bool executeQuery(const std::string& query) {
    if (char* errorMessage = nullptr;
        sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Ошибка SQL: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
    }

    bool isVehicleInDatabase(const std::string& licensePlate) {
    std::string query = "SELECT COUNT(*) FROM Vehicles WHERE licensePlate = '" + licensePlate + "';";
    sqlite3_stmt* stmt;
    bool exists = false;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            exists = sqlite3_column_int(stmt, 0) > 0;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Ошибка SQL при проверке дублирования: " << sqlite3_errmsg(db) << std::endl;
    }

    return exists;
    }

    sqlite3* getDB() const { return db; }

private:
    sqlite3* db = nullptr;
};
#endif 
