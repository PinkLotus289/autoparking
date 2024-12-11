#include "../Header/ExportManager.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <xlsxwriter.h>
#include <iostream>

void ExportManager::exportVehicles(const QString& format, const QString& filePath, 
                                   const std::vector<std::shared_ptr<Vehicle>>& vehicles, 
                                   const FilterCriteria& criteria) {
    std::vector<std::shared_ptr<Vehicle>> filteredVehicles;
    for (const auto& vehicle : vehicles) {
        bool matches = (criteria.includeFreeVehicles && !vehicle->getStatus()) ||
                       (criteria.includeParkedVehicles && vehicle->getStatus());

        if (matches) {
            filteredVehicles.push_back(vehicle);
        }
    }

    if (format == "JSON") {
        QJsonArray jsonArray;
        for (const auto& vehicle : filteredVehicles) {
            QJsonObject obj;
            obj["Model"] = QString::fromStdString(vehicle->getModel());
            obj["LicensePlate"] = QString::fromStdString(vehicle->getLicensePlate());
            obj["Status"] = vehicle->getStatus() ? "Запаркована" : "Свободна";
            jsonArray.append(obj);
        }
        QJsonDocument doc(jsonArray);
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(doc.toJson());
            file.close();
        }
    }
    // Аналогично для других форматов (XML, XLSX, DOCX)
}

void ExportManager::exportParkingSpots(const QString& format, const QString& filePath, 
                                       const std::vector<std::shared_ptr<ParkingSpot>>& spots, 
                                       const FilterCriteria& criteria) {
    std::vector<std::shared_ptr<ParkingSpot>> filteredSpots;
    for (const auto& spot : spots) {
        bool matches = (spot->getNumber() >= criteria.numberFrom && spot->getNumber() <= criteria.numberTo) &&
                       (criteria.size.isEmpty() || spot->getSize() == criteria.size.toStdString()) &&
                       (criteria.isOccupied == spot->getStatus());

        if (matches) {
            filteredSpots.push_back(spot);
        }
    }

    if (format == "JSON") {
        QJsonArray jsonArray;
        for (const auto& spot : filteredSpots) {
            QJsonObject obj;
            obj["Number"] = spot->getNumber();
            obj["Size"] = QString::fromStdString(spot->getSize());
            obj["Status"] = spot->getStatus() ? "Занято" : "Свободно";
            jsonArray.append(obj);
        }
        QJsonDocument doc(jsonArray);
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(doc.toJson());
            file.close();
        }
    }
    // Аналогично для других форматов (XML, XLSX, DOCX)
}

void ExportManager::exportData(const QString &filePath, 
                               const std::vector<std::shared_ptr<Vehicle>> &vehicles, 
                               const std::vector<std::shared_ptr<ParkingSpot>> &spots) {
    if (filePath.endsWith(".json")) {
        // Экспорт в JSON
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly)) {
            throw std::runtime_error("Не удалось открыть файл для записи.");
        }

        QJsonArray vehiclesArray;
        for (const auto &vehicle : vehicles) {
            QJsonObject vehicleObject;
            vehicleObject["licensePlate"] = QString::fromStdString(vehicle->getLicensePlate());
            vehicleObject["model"] = QString::fromStdString(vehicle->getModel());
            vehicleObject["status"] = vehicle->getStatus() ? "Запарковано" : "Свободно";
            vehiclesArray.append(vehicleObject);
        }

        QJsonArray spotsArray;
        for (const auto &spot : spots) {
            QJsonObject spotObject;
            spotObject["number"] = spot->getNumber();
            spotObject["size"] = QString::fromStdString(spot->getSize());
            spotObject["occupied"] = spot->getStatus() ? "Занято" : "Свободно";
            spotsArray.append(spotObject);
        }

        QJsonObject rootObject;
        rootObject["vehicles"] = vehiclesArray;
        rootObject["spots"] = spotsArray;

        QJsonDocument doc(rootObject);
        file.write(doc.toJson());
        file.close();
    } else if (filePath.endsWith(".xml")) {
        // Экспорт в XML
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly)) {
            throw std::runtime_error("Не удалось открыть файл для записи.");
        }

        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("ParkingLot");

        xmlWriter.writeStartElement("Vehicles");
        for (const auto &vehicle : vehicles) {
            xmlWriter.writeStartElement("Vehicle");
            xmlWriter.writeTextElement("LicensePlate", QString::fromStdString(vehicle->getLicensePlate()));
            xmlWriter.writeTextElement("Model", QString::fromStdString(vehicle->getModel()));
            xmlWriter.writeTextElement("Status", vehicle->getStatus() ? "Запарковано" : "Свободно");
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("ParkingSpots");
        for (const auto &spot : spots) {
            xmlWriter.writeStartElement("Spot");
            xmlWriter.writeTextElement("Number", QString::number(spot->getNumber()));
            xmlWriter.writeTextElement("Size", QString::fromStdString(spot->getSize()));
            xmlWriter.writeTextElement("Occupied", spot->getStatus() ? "Занято" : "Свободно");
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();

        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
        file.close();
    } else {
        throw std::runtime_error("Формат файла не поддерживается.");
    }
}
