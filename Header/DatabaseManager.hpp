// DatabaseManager.hpp
#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <sqlite3.h>
#include <string>
#include <iostream>

class DatabaseManager {
public:
    DatabaseManager(const std::string& dbFile) {
        if (sqlite3_open(dbFile.c_str(), &db) != SQLITE_OK) {
            std::cerr << "Ошибка при открытии базы данных: " << sqlite3_errmsg(db) << std::endl;
        } else {
            std::cout << "Подключение к базе данных успешно." << std::endl;
        }
    }

    ~DatabaseManager() {
        sqlite3_close(db);
    }

    bool executeQuery(const std::string& query) {
        char* errorMessage;
        if (sqlite3_exec(db, query.c_str(), NULL, NULL, &errorMessage) != SQLITE_OK) {
            std::cerr << "Ошибка SQL: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
            return false;
        }
        return true;
    }

    sqlite3* getDB() const { return db; }

private:
    sqlite3* db;
};

#endif // DATABASEMANAGER_HPP
