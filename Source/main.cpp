#include <iostream>
#include <memory>
#include "../Header/ParkingLot.hpp"

using namespace std;

void displayMenu() {
    cout << "Меню:" << endl;
    cout << "1. Отобразить информацию о парковке (обычный пользователь)" << endl;
    cout << "2. Отобразить информацию о парковке (администратор)" << endl;
    cout << "3. Удалить машину" << endl;
    cout << "4. Удалить парковочное место" << endl;
    cout << "5. Выход" << endl;
    cout << "6. Внести информацию о парковочном месте" << endl;
    cout << "Введите ваш выбор: ";
}

int main() {
    setlocale(LC_ALL, "Russian");

    auto parkingLot = make_unique<ParkingLot>("Центральная парковка");

    auto car1 = make_unique<Car>("Toyota", "А123ВС77");
    auto car2 = make_unique<Car>("Tesla", "Х456УУ99", true);
    parkingLot->addCar(std::move(car1));
    parkingLot->addCar(std::move(car2));

    auto spot1 = make_unique<ParkingSpot>(101, "Компактное", true);
    auto spot2 = make_unique<ParkingSpot>(202, "Большое", false);
    parkingLot->addParkingSpot(std::move(spot1));
    parkingLot->addParkingSpot(std::move(spot2));

    int choice;
    bool running = true;

    while (running) {
        displayMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                cout << "\nИнформация для обычного пользователя:" << endl;
                parkingLot->displayParkingLot(false);
                break;
            case 2:
                cout << "\nИнформация для администратора:" << endl;
                parkingLot->displayParkingLot(true);
                break;
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
                parkingLot->removeParkingSpot(spotNumber);
                break;
            }
            case 5:
                running = false;
                break;
            case 6: { // Новый пункт меню для добавления парковочного места
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

                auto newSpot = std::make_unique<ParkingSpot>(number, size, occupied);
                parkingLot->addParkingSpot(std::move(newSpot));

                cout << "Парковочное место добавлено!" << endl;
                break;
            }
            default:
                cout << "Неверный выбор, попробуйте снова." << endl;
                break;
        }
        cout << endl;
    }

    cout << "Завершение программы..." << endl;
    return 0;
}