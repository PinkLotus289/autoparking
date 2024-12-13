#ifndef INPUTVALIDATOR_HPP
#define INPUTVALIDATOR_HPP

#include <QString>
#include <QRegularExpression>
#include <QMessageBox>
#include <QDateTime>
#include <QInputDialog>
#include "../Exceptions/ValidationError.hpp"
#include "../Exceptions/InvalidTimeFormatError.hpp"
#include "../Exceptions/InvalidLicensePlateError.hpp"
#include "../Exceptions/InputCancelledError.hpp"
#include "../Exceptions/DuplicateVehicleError.hpp"
#include "../Exceptions/InvalidSpotNumberError.hpp"

class InputValidator {
public:
    static bool isValidTimeFormat(const QString& time) {
        QRegularExpression timeFormatRegex(
            R"(^\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])\s(0[0-9]|1[0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])$)"
        );
        return timeFormatRegex.match(time).hasMatch();
    }

    static QDateTime parseTime(const QString& time) {
        if (!isValidTimeFormat(time)) {
            throw InvalidTimeFormatError();
        }

        QDateTime dateTime = QDateTime::fromString(time, "yyyy-MM-dd HH:mm:ss");
        if (!dateTime.isValid()) {
            throw ValidationError("Ошибка преобразования строки в QDateTime.");
        }

        return dateTime;
    }

    static QString getValidatedTime(QWidget* parent) {
        bool ok;
        QString inputTime;

        do {
            inputTime = QInputDialog::getText(
                parent,
                "Ввод времени парковки",
                "Введите время парковки (формат: YYYY-MM-DD HH:mm:ss):",
                QLineEdit::Normal,
                "",
                &ok
            );

            if (!ok) {
                throw InputCancelledError();
            }

            try {
                parseTime(inputTime);
                return inputTime;
            } catch (const ValidationError& e) {
                QMessageBox::warning(parent, "Ошибка", e.what());
            }
        } while (true);
    }

    static int getValidatedSpotNumber(QWidget* parent, int min, int max) {
        bool ok;
        int spotNumber;

        do {
            spotNumber = QInputDialog::getInt(
                parent,
                "Ввод номера парковочного места",
                QString("Введите номер парковочного места (%1 - %2):").arg(min).arg(max),
                min,
                min,
                max,
                1,
                &ok
            );

            if (!ok) {
                throw InputCancelledError();
            }

            if (spotNumber >= min && spotNumber <= max) {
                return spotNumber;
            } else {
                throw InvalidSpotNumberError();
            }
        } while (true);
    }

    static bool isValidLicensePlate(const QString& licensePlate) {
        QRegularExpression regex(R"(^\d{4}[А-Я]{2}-\d$)");
        return regex.match(licensePlate).hasMatch();
    }

    static QString getValidatedLicensePlate(QWidget* parent) {
        bool ok;
        QString licensePlate;

        do {
            licensePlate = QInputDialog::getText(
                parent,
                "Ввод номерного знака",
                "Введите номерной знак (формат: 1234ГС-5):",
                QLineEdit::Normal,
                "",
                &ok
            );

            if (!ok) {
                throw InputCancelledError();
            }

            if (isValidLicensePlate(licensePlate)) {
                return licensePlate;
            } else {
                throw InvalidLicensePlateError();
            }
        } while (true);
    }

    static bool isVehicleInMemory(const std::string& licensePlate, const std::vector<std::shared_ptr<Vehicle>>& vehicles) {
        for (const auto& vehicle : vehicles) {
            if (vehicle->getLicensePlate() == licensePlate) {
                throw DuplicateVehicleError();
            }
        }
        return false;
    }
};

#endif // INPUTVALIDATOR_HPP
