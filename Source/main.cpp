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
    cout << "5. Добавить новое авто" << endl;
    cout << "6. Внести информацию о парковочном месте" << endl;
    cout << "7. Запарковать автомобиль" << endl;
    cout << "8. Освободить парковочное место" << endl;
    cout << "9. Выход" << endl;
    cout << "Введите ваш выбор: ";
}

int main() {
    setlocale(LC_ALL, "Russian");

    auto parkingLot = make_unique<ParkingLot>("Центральная парковка");

    auto car1 = make_unique<Car>("Toyota", "А123ВС77");
    auto car2 = make_unique<Car>("Tesla", "Х456УУ99");
    parkingLot->addCar(std::move(car1));
    parkingLot->addCar(std::move(car2));

    auto spot1 = make_unique<ParkingSpot>(101, "Компактное", false);
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
            case 1:{
                cout << "\nИнформация для обычного пользователя:" << endl;
                parkingLot->displayParkingLot(false);
                break;
            }
            case 2:{
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
                parkingLot->removeParkingSpot(spotNumber);
                break;
            }
            case 5: {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                std::cout << "\nВведите модель авто: ";
                std::string model;
                std::getline(std::cin, model); 

                std::cout << "Введите номерной знак авто: ";
                std::string licensePlate;
                std::getline(std::cin, licensePlate); 

                auto car = std::make_shared<Car>(model, licensePlate);
                parkingLot->addCar(car);
                std::cout << "Автомобиль успешно добавлен!\n";
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

                auto newSpot = std::make_unique<ParkingSpot>(number, size, occupied);
                parkingLot->addParkingSpot(std::move(newSpot));

                cout << "Парковочное место добавлено!" << endl;
                break;
            } case 7: {
                std::cout << "\nВведите номерной знак машины для закрепления: ";
                std::string licensePlate;
                std::getline(std::cin, licensePlate);

                std::cout << "Введите номер парковочного места: ";
                int spotNumber;
                std::cin >> spotNumber;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                parkingLot->assignCarToSpot(licensePlate, spotNumber);
                break;
            } case 8: {
                std::cout << "\nВведите номер парковочного места для освобождения: ";
                int spotNumber;
                std::cin >> spotNumber;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                parkingLot->releaseParkingSpot(spotNumber);
                break;
            }
            case 9:
                running = false;
                break;
            default:
                cout << "Неверный выбор, попробуйте снова." << endl;
                break;
        }
        cout << endl;
    }

    cout << "Завершение программы..." << endl;
    return 0;
}