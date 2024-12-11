#ifndef INPUTVALIDATOR_HPP
#define INPUTVALIDATOR_HPP

#include <QString>
#include <QRegularExpression>
#include <QMessageBox>
#include <QDateTime>
#include <stdexcept>
#include <QInputDialog>

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
            throw std::invalid_argument("Неверный формат времени. Ожидаемый формат: YYYY-MM-DD HH:mm:ss");
        }

        QDateTime dateTime = QDateTime::fromString(time, "yyyy-MM-dd HH:mm:ss");
        if (!dateTime.isValid()) {
            throw std::invalid_argument("Ошибка преобразования строки в QDateTime.");
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
                return ""; 
            }

            try {
                parseTime(inputTime);
                return inputTime;
            } catch (const std::invalid_argument& e) {
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
                throw std::runtime_error("Ввод отменён пользователем.");
            }

            if (spotNumber >= min && spotNumber <= max) {
                return spotNumber; 
            } else {
                QMessageBox::warning(parent, "Ошибка", "Номер парковочного места вне допустимого диапазона.");
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
                throw std::runtime_error("Ввод номерного знака отменён пользователем.");
            }

            if (isValidLicensePlate(licensePlate)) {
                return licensePlate; 
            } else {
                QMessageBox::warning(parent, "Ошибка", "Неверный формат номерного знака. Ожидаемый формат: 1234ГС-5.");
            }
        } while (true);
    }

    static bool isVehicleInMemory(const std::string& licensePlate, const std::vector<std::shared_ptr<Vehicle>>& vehicles) {
        for (const auto& vehicle : vehicles) {
            if (vehicle->getLicensePlate() == licensePlate) {
                return true; 
            }
        }
        return false;
    }

};

#endif 
