#include "../Header/ExportManager.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <xlsxwriter.h>
#include <iostream>
#include <stdexcept>
#include <fstream>

void ExportManager::exportVehicles(const QString &filePath,
                                   const std::vector<std::shared_ptr<Vehicle>> &vehicles,
                                   const FilterCriteria &criteria) {
    QJsonArray jsonArray;

    for (const auto &vehicle : vehicles) {
        if ((criteria.includeFreeVehicles && !vehicle->getStatus()) ||
            (criteria.includeParkedVehicles && vehicle->getStatus())) {
            QJsonObject obj;
            obj["Model"] = QString::fromStdString(vehicle->getModel());
            obj["LicensePlate"] = QString::fromStdString(vehicle->getLicensePlate());
            obj["Status"] = vehicle->getStatus() ? "Запаркован" : "Свободен";
            jsonArray.append(obj);
        }
    }

    QJsonObject root;
    root["Vehicles"] = jsonArray;

    QJsonDocument doc(root);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Не удалось открыть файл для записи.");
    }
    file.write(doc.toJson());
    file.close();
}

void ExportManager::exportParkingSpots(const QString &filePath,
                                       const std::vector<std::shared_ptr<ParkingSpot>> &spots,
                                       const FilterCriteria &criteria) {
    QJsonArray jsonArray;

    for (const auto &spot : spots) {
        if (((criteria.includeFreeSpots && !spot->getStatus()) ||
             (criteria.includeOccupiedSpots && spot->getStatus())) &&
            (criteria.size.isEmpty() || QString::fromStdString(spot->getSize()) == criteria.size) &&
            spot->getNumber() >= criteria.numberFrom && spot->getNumber() <= criteria.numberTo) {
            QJsonObject obj;
            obj["Number"] = spot->getNumber();
            obj["Size"] = QString::fromStdString(spot->getSize());
            obj["Status"] = spot->getStatus() ? "Занято" : "Свободно";
            jsonArray.append(obj);
        }
    }

    QJsonObject root;
    root["ParkingSpots"] = jsonArray;

    QJsonDocument doc(root);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Не удалось открыть файл для записи.");
    }
    file.write(doc.toJson());
    file.close();
}


void ExportManager::exportData(const QString &filePath,
                               const std::vector<std::shared_ptr<Vehicle>> &vehicles,
                               const std::vector<std::shared_ptr<ParkingSpot>> &spots,
                               const FilterCriteria &criteria) {
    QJsonObject rootObject;


    if (criteria.includeFreeVehicles || criteria.includeParkedVehicles) {
        QJsonArray vehiclesArray;
        for (const auto &vehicle : vehicles) {
            if ((criteria.includeFreeVehicles && !vehicle->getStatus()) ||
                (criteria.includeParkedVehicles && vehicle->getStatus())) {
                QJsonObject vehicleObject;
                vehicleObject["Model"] = QString::fromStdString(vehicle->getModel());
                vehicleObject["LicensePlate"] = QString::fromStdString(vehicle->getLicensePlate());
                vehicleObject["Status"] = vehicle->getStatus() ? "Запаркован" : "Свободен";
                vehiclesArray.append(vehicleObject);
            }
        }
        rootObject["Vehicles"] = vehiclesArray;
    }


    if (criteria.includeFreeSpots || criteria.includeOccupiedSpots) {
        QJsonArray spotsArray;
        for (const auto &spot : spots) {
            if (((criteria.includeFreeSpots && !spot->getStatus()) ||
                 (criteria.includeOccupiedSpots && spot->getStatus())) &&
                (criteria.size.isEmpty() || QString::fromStdString(spot->getSize()) == criteria.size) &&
                spot->getNumber() >= criteria.numberFrom && spot->getNumber() <= criteria.numberTo) {
                QJsonObject spotObject;
                spotObject["Number"] = spot->getNumber();
                spotObject["Size"] = QString::fromStdString(spot->getSize());
                spotObject["Status"] = spot->getStatus() ? "Занято" : "Свободно";
                spotsArray.append(spotObject);
            }
        }
        rootObject["ParkingSpots"] = spotsArray;
    }

    QJsonDocument doc(rootObject);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Не удалось открыть файл для записи.");
    }
    file.write(doc.toJson());
    file.close();
}
