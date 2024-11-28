#include "../Header/MainWindow.hpp"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QTextEdit>
#include <QInputDialog>
#include <QLineEdit>
#include <iostream>
#include <memory>
#include "../Header/Car.hpp"
#include "../Header/Truck.hpp" 
#include "../Header/Truck.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), dbManager("../parking_lot.db") {
    if (int rc = sqlite3_open("../parking_lot.db", &db); rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    parkingLot = std::make_unique<ParkingLot>("Центральная парковка", dbManager);
    std::cout << "Объект ParkingLot успешно создан." << std::endl;

    auto *centralWidget = new QWidget(this);
    auto *layout = new QVBoxLayout(centralWidget);

    auto *label = new QLabel("Меню парковки", this);
    layout->addWidget(label);


    auto *displayButtonUser = new QPushButton("Отобразить информацию о парковке (не админ)", this);
    auto *displayButtonAdmin = new QPushButton("Отобразить информацию о парковке (админ)", this);
    auto *removeVehicleButton = new QPushButton("Удалить транспортное средство", this); 
    auto *removeSpotButton = new QPushButton("Удалить парковочное место", this);
    auto *addVehicleButton = new QPushButton("Добавить транспортное средство", this); 
    auto *addSpotButton = new QPushButton("Добавить парковочное место", this);
    auto *assignVehicleButton = new QPushButton("Назначить транспортное средство на место", this); 
    auto *releaseSpotButton = new QPushButton("Освободить место", this);
    auto *sortSpotsButton = new QPushButton("Отсортировать парковочные места", this);
    auto *enqueueButton = new QPushButton("Поставить в очередь", this);
    auto *parkFromQueueButton = new QPushButton("Запарковать из очереди", this);
    
    
    layout->addWidget(displayButtonUser);
    layout->addWidget(displayButtonAdmin);
    layout->addWidget(removeVehicleButton);
    layout->addWidget(removeSpotButton);
    layout->addWidget(addVehicleButton);
    layout->addWidget(addSpotButton);
    layout->addWidget(assignVehicleButton);
    layout->addWidget(releaseSpotButton);
    layout->addWidget(sortSpotsButton);
    layout->addWidget(enqueueButton);
    layout->addWidget(parkFromQueueButton);

    
    infoDisplay = new QTextEdit(this);
    infoDisplay->setReadOnly(true);
    layout->addWidget(infoDisplay);

    setCentralWidget(centralWidget);

    
    connect(displayButtonUser, &QPushButton::clicked, this, &MainWindow::displayParkingInfoUser);
    connect(displayButtonAdmin, &QPushButton::clicked, this, &MainWindow::displayParkingInfoAdmin);
    connect(removeVehicleButton, &QPushButton::clicked, this, &MainWindow::removeVehicle); 
    connect(removeSpotButton, &QPushButton::clicked, this, &MainWindow::removeParkingSpot);
    connect(addVehicleButton, &QPushButton::clicked, this, &MainWindow::addVehicle); 
    connect(addSpotButton, &QPushButton::clicked, this, &MainWindow::addParkingSpot);
    connect(assignVehicleButton, &QPushButton::clicked, this, &MainWindow::assignVehicleToSpot); 
    connect(releaseSpotButton, &QPushButton::clicked, this, &MainWindow::releaseParkingSpot);
    connect(sortSpotsButton, &QPushButton::clicked, this, &MainWindow::sortAndSaveSpotsToDatabase);
    connect(enqueueButton, &QPushButton::clicked, this, &MainWindow::enqueueVehicle);
    connect(parkFromQueueButton, &QPushButton::clicked, this, &MainWindow::parkVehicleFromQueue);
}

MainWindow::~MainWindow() {
    sqlite3_close(db);
}


void MainWindow::displayParkingInfoUser() {
    QString info = "Информация о парковке для пользователей:\n";
    info += generateParkingInfo(false);
    infoDisplay->setText(info);
}

void MainWindow::displayParkingInfoAdmin() {
    QString info = "Информация о парковке для администратора:\n";
    info += generateParkingInfo(true);
    infoDisplay->setText(info);
}

QString MainWindow::generateParkingInfo(bool isAdmin) {
    QString info;
    info += "\nТранспортные средства на парковке:\n";

    for (const auto &vehicle : parkingLot->getVehicles()) {
        info += QString::fromStdString(vehicle->getModel()) + " | " +
                QString::fromStdString(vehicle->getLicensePlate()) + " | " +
                (vehicle->getStatus() ? "Занято" : "Свободно") + "\n";
    }

    info += "\nПарковочные места:\n";

    for (const auto &spot : parkingLot->getSpots()) {
        info += QString("Место %1 | Размер: %2 | Статус: %3 | Транспорт: %4\n")
                    .arg(spot->getNumber())
                    .arg(QString::fromStdString(spot->getSize()))
                    .arg(spot->getStatus() ? "Занято" : "Свободно")
                    .arg(spot->getVehicle() ? QString::fromStdString(spot->getVehicle()->getLicensePlate()) : "Нет");
    }

    double freePercentage = calculateFreeSpotPercentage(*parkingLot);
    info += QString("\nПроцент свободных парковочных мест: %1%\n").arg(freePercentage, 0, 'f', 2);

    return info;
}


void MainWindow::addParkingSpot() {
    bool ok;
    int number = QInputDialog::getInt(this, "Добавить парковочное место", "Введите номер:", 1, 1, 10000, 1, &ok);
    if (!ok) return;

    QString size = QInputDialog::getText(this, "Добавить парковочное место", "Введите размер:", QLineEdit::Normal, "маленький", &ok);
    if (!ok || size.isEmpty()) return;

    auto spot = std::make_shared<ParkingSpot>(number, size.toStdString(), false);
    parkingLot->addParkingSpot(spot, dbManager);

}

void MainWindow::removeParkingSpot() {
    bool ok;
    int spotNumber = QInputDialog::getInt(this, "Удалить парковочное место", 
                                          "Введите номер места:", 1, 1, 10000, 1, &ok);
    if (!ok) return;

    parkingLot->removeParkingSpot(spotNumber);
}

void MainWindow::assignVehicleToSpot() {
    bool ok;
    QString licensePlate = QInputDialog::getText(this, "Назначить транспорт", "Введите номерной знак:", QLineEdit::Normal, "", &ok);
    if (!ok || licensePlate.isEmpty()) return;

    int spotNumber = QInputDialog::getInt(this, "Назначить транспорт", "Введите номер места:", 1, 1, 10000, 1, &ok);
    if (!ok) return;

    parkingLot->assignVehicleToSpot(licensePlate.toStdString(), spotNumber);
}

void MainWindow::addVehicle() {
    QStringList types = {"Автомобиль", "Грузовик"};
    bool ok;
    QString type = QInputDialog::getItem(this, "Добавить транспортное средство", "Выберите тип:", types, 0, false, &ok);
    if (!ok || type.isEmpty()) return;

    QString licensePlate = QInputDialog::getText(this, "Добавить транспортное средство", "Введите номерной знак:", QLineEdit::Normal, "", &ok);
    if (!ok || licensePlate.isEmpty()) return;

    std::shared_ptr<Vehicle> vehicle;
    if (type == "Автомобиль") {
        QString model = QInputDialog::getText(this, "Добавить автомобиль", "Введите модель:", QLineEdit::Normal, "", &ok);
        if (!ok || model.isEmpty()) return;
        vehicle = std::make_shared<Car>(model.toStdString(), licensePlate.toStdString());
    } else if (type == "Грузовик") {
        QString cargo = QInputDialog::getText(this, "Добавить грузовик", "Введите тип груза:", QLineEdit::Normal, "", &ok);
        if (!ok || cargo.isEmpty()) return;
        vehicle = std::make_shared<Truck>(cargo.toStdString(), licensePlate.toStdString());
    }

    parkingLot->addVehicle(vehicle);
}

void MainWindow::removeVehicle() {
    bool ok;
    QString licensePlate = QInputDialog::getText(this, "Удалить транспортное средство", "Введите номерной знак:", QLineEdit::Normal, "", &ok);
    if (!ok || licensePlate.isEmpty()) return;

    parkingLot->removeVehicle(licensePlate.toStdString());
}

void MainWindow::releaseParkingSpot() {
    bool ok;
    int spotNumber = QInputDialog::getInt(this, "Освободить место", "Введите номер:", 1, 1, 10000, 1, &ok);
    if (!ok) return;

    parkingLot->releaseParkingSpot(spotNumber);
}

void MainWindow::sortAndSaveSpotsToDatabase() {
    parkingLot->sortAndSaveSpotsToDatabase();
}

void MainWindow::enqueueVehicle() {
    QStringList types = {"Легковой автомобиль", "Грузовой автомобиль"};
    bool ok;
    QString type = QInputDialog::getItem(this, "Поставить в очередь", "Выберите тип автомобиля:", types, 0, false, &ok);
    if (!ok || type.isEmpty()) return;

    QString licensePlate = QInputDialog::getText(this, "Поставить в очередь", "Введите номерной знак:", QLineEdit::Normal, "", &ok);
    if (!ok || licensePlate.isEmpty()) return;

    if (type == "Легковой автомобиль") {
        QString model = QInputDialog::getText(this, "Поставить в очередь", "Введите модель автомобиля:", QLineEdit::Normal, "", &ok);
        if (!ok || model.isEmpty()) return;

        auto car = std::make_shared<Car>(model.toStdString(), licensePlate.toStdString());
        carQueue.enqueue(car);
        infoDisplay->append("Легковой автомобиль добавлен в очередь.");
    } else if (type == "Грузовой автомобиль") {
        QString cargoType = QInputDialog::getText(this, "Поставить в очередь", "Введите тип груза:", QLineEdit::Normal, "", &ok);
        if (!ok || cargoType.isEmpty()) return;

        auto truck = std::make_shared<Truck>(cargoType.toStdString(), licensePlate.toStdString());
        truckQueue.enqueue(truck);
        infoDisplay->append("Грузовой автомобиль добавлен в очередь.");
    }
}

void MainWindow::parkVehicleFromQueue() {
    QStringList queues = {"Очередь легковых автомобилей", "Очередь грузовых автомобилей"};
    bool ok;

    // Выбор очереди
    QString queueType = QInputDialog::getItem(this, "Запарковать автомобиль", "Выберите очередь:", queues, 0, false, &ok);
    if (!ok || queueType.isEmpty()) return;

    // Ввод номера парковочного места
    int spotNumber = QInputDialog::getInt(this, "Запарковать автомобиль", "Введите номер парковочного места:", 1, 1, 10000, 1, &ok);
    if (!ok) return;

    try {
        std::shared_ptr<Vehicle> vehicle;

        // Извлекаем автомобиль из соответствующей очереди
        if (queueType == "Очередь легковых автомобилей") {
            if (carQueue.isEmpty()) {
                QMessageBox::warning(this, "Ошибка", "Очередь легковых автомобилей пуста.");
                return;
            }
            vehicle = carQueue.dequeue();
        } else if (queueType == "Очередь грузовых автомобилей") {
            if (truckQueue.isEmpty()) {
                QMessageBox::warning(this, "Ошибка", "Очередь грузовых автомобилей пуста.");
                return;
            }
            vehicle = truckQueue.dequeue();
        }

        // Добавляем автомобиль в базу данных
        std::string type = std::dynamic_pointer_cast<Car>(vehicle) ? "Car" : "Truck";
        std::string insertVehicleQuery = std::format(
            "INSERT INTO Vehicles (type, model, licensePlate, status) VALUES ('{}', '{}', '{}', 0);",
            type, vehicle->getModel(), vehicle->getLicensePlate()
        );

        if (!parkingLot->getDatabaseManager().executeQuery(insertVehicleQuery)) {
            throw std::runtime_error("Ошибка при добавлении автомобиля в базу данных.");
        }

        // Обновляем данные в памяти
        parkingLot->loadVehiclesFromDatabase();

        // Привязываем автомобиль к парковочному месту
        parkingLot->assignVehicleToSpot(vehicle->getLicensePlate(), spotNumber);

        infoDisplay->append(QString("Автомобиль с номером %1 запаркован на месте %2.")
                            .arg(QString::fromStdString(vehicle->getLicensePlate()))
                            .arg(spotNumber));
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}
