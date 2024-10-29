#include <iostream>
#include <memory>
#include <iomanip>
#include "../Header/ParkingLot.hpp"
#include "../Header/DatabaseManager.hpp"

using namespace std;

void displayMenu() {
    cout << "Меню:" << endl;
    cout << "1. Отобразить информацию о парковке (обычный пользователь)" << endl;
    cout << "2. Отобразить информацию о парковке (администратор)" << endl;
    cout << "3. Удалить машину" << endl;
    cout << "4. Удалить парковочное место" << endl;
    cout << "5. Добавить новое авто" << endl;
    cout << "6. Внести информацию о парковочном месте" << endl;
    cout << "7. Запарковать автомобиль" << endl;
    cout << "8. Освободить парковочное место" << endl;
    cout << "9. Найти аналогичные свободные парковочные места" << endl;
    cout << "10. Выход" << endl;
    cout << "Введите ваш выбор: ";
}

void handleInvalidChoice() {
    std::cout << "Неверный выбор, попробуйте снова." << std::endl;
}


int main() {
    DatabaseManager dbManager("parking_lot.db"); // Создаем объект DatabaseManager для работы с БД
    setlocale(LC_ALL, "Russian");

    auto parkingLot = make_unique<ParkingLot>("Центральная парковка", dbManager); // Передаем dbManager в ParkingLot
    
    int choice;
    bool running = true;

    while (running) {
        displayMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                cout << "\nИнформация для обычного пользователя:" << endl;
                parkingLot->displayParkingLot(false);
                break;
            }
            case 2: {
                cout << "\nИнформация для администратора:" << endl;
                parkingLot->displayParkingLot(true);
                break;
            }
            case 3: {
                cout << "\nВведите номерной знак машины для удаления: ";
                string licensePlate;
                getline(cin, licensePlate);
                parkingLot->removeCar(licensePlate);
                break;
            }
            case 4: {
                cout << "\nВведите номер парковочного места для удаления: ";
                int spotNumber;
                cin >> spotNumber;
                *parkingLot -= spotNumber;
                break;
            }
            case 5: {
                cout << "\nВведите модель авто: ";
                string model;
                getline(cin, model);

                cout << "Введите номерной знак авто: ";
                string licensePlate;
                getline(cin, licensePlate);

                auto car = make_shared<Car>(model, licensePlate);
                parkingLot->addCar(car);
                cout << "Автомобиль успешно добавлен!\n";
                break;
            }
            case 6: {
                cout << "\nВведите номер парковочного места: ";
                int number;
                cin >> number;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Введите размер парковочного места (например, Компактное, Большое): ";
                string size;
                getline(cin, size);

                cout << "Занято ли место? (1 - Да, 0 - Нет): ";
                bool occupied;
                cin >> occupied;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                auto newSpot = make_shared<ParkingSpot>(number, size, occupied);
                *parkingLot += newSpot;

                cout << "Парковочное место добавлено!" << endl;
                break;
            }
            case 7: {
                std::cout << "\nВведите номерной знак машины для закрепления: ";
                std::string licensePlate;
                std::getline(std::cin, licensePlate);

                std::cout << "Введите номер парковочного места: ";
                int spotNumber;
                std::cin >> spotNumber;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                parkingLot->assignCarToSpot(licensePlate, spotNumber);  // Закрепляем машину за местом и обновляем статус
                break;
            }
            case 8: {
                std::cout << "\nВведите номер парковочного места для освобождения: ";
                int spotNumber;
                std::cin >> spotNumber;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                parkingLot->releaseParkingSpot(spotNumber);  // Освобождаем место и обновляем статус автомобиля
                break;
            }
            case 9: {  // Новый case для вывода аналогичных свободных мест
                std::cout << "Введите номер парковочного места для поиска аналогичных свободных мест: ";
                int spotNumber;
                std::cin >> spotNumber;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (const ParkingSpot* selectedSpot = parkingLot->getParkingSpot(spotNumber)) {
                    std::cout << "\nСвободные парковочные места аналогичного размера (" << selectedSpot->getSize() << "):\n";
                    std::cout << std::left << std::setw(10) << "Номер"
                            << std::left << std::setw(30) << "Размер" << std::endl;
                    std::cout << std::string(40, '-') << std::endl;

                    // Используем метод getParkingSpots() для получения списка мест
                    for (const auto& spot : parkingLot->getParkingSpots()) {
                        if (!spot->isOccupied() && *spot == *selectedSpot) {
                            std::cout << std::left << std::setw(10) << spot->getNumber()
                                    << std::left << std::setw(30) << spot->getSize()
                                    << std::endl;
                        }
                    }
                } else {
                    std::cout << "Парковочное место с таким номером не найдено.\n";
                }
                break;
            }
            case 10: {
                running = false;
                break;
            }
            default:{
                handleInvalidChoice();
                break;
            }
    }
    }
    cout << "Завершение программы..." << endl;
    return 0;
}