#include "../Header/MainWindow.hpp"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QTextEdit>
#include <QInputDialog>
#include <QLineEdit>
#include <QHeaderView>
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
    auto *returnButton = new QPushButton("Вернуться в начальное меню", this);
    
    
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
    layout->addWidget(returnButton);

    
    // Создаём таблицу для автомобилей
    vehicleTable = new QTableWidget(this);
    setupVehicleTable();
    layout->addWidget(new QLabel("Автомобили", this));
    layout->addWidget(vehicleTable);

    // Создаём таблицу для парковочных мест
    spotTable = new QTableWidget(this);
    setupSpotTable();
    layout->addWidget(new QLabel("Парковочные места", this));
    layout->addWidget(spotTable);

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
    connect(returnButton, &QPushButton::clicked, this, &MainWindow::onReturnToMenuClicked);
}

MainWindow::~MainWindow() {
    sqlite3_close(db);
}

void MainWindow::setupVehicleTable() {
    vehicleTable->setColumnCount(4);
    vehicleTable->setHorizontalHeaderLabels({"Марка авто", "Регистрационный номер", "Статус", "Номер места"});
    vehicleTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    vehicleTable->verticalHeader()->setVisible(false);
    vehicleTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    vehicleTable->setSelectionMode(QAbstractItemView::NoSelection);
}

void MainWindow::setupSpotTable() {
    spotTable->setColumnCount(4);
    spotTable->setHorizontalHeaderLabels({"Номер места", "Размер", "Статус", "Номер авто"});
    spotTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    spotTable->verticalHeader()->setVisible(false);
    spotTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    spotTable->setSelectionMode(QAbstractItemView::NoSelection);
}


void MainWindow::displayParkingInfoUser() {
    // Очистка таблицы парковочных мест
    spotTable->setRowCount(0);
    vehicleTable->setRowCount(0);

    int row = 0;

    // Сначала добавляем строку с процентом свободных мест
    double freePercentage = calculateFreeSpotPercentage(*parkingLot);
    spotTable->insertRow(row);
    spotTable->setItem(row, 0, new QTableWidgetItem("Процент свободных мест:"));
    spotTable->setItem(row, 1, new QTableWidgetItem(QString::number(freePercentage, 'f', 2) + " %"));
    spotTable->setSpan(row, 1, 1, 3); // Объединяем ячейки для текста
    row++;

    // Затем выводим список свободных мест
    for (const auto &spot : parkingLot->getSpots()) {
        if (!spot->getStatus()) { // Условие: только свободные места
            spotTable->insertRow(row);

            // Номер места
            spotTable->setItem(row, 0, new QTableWidgetItem(QString::number(spot->getNumber())));
            // Размер
            spotTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(spot->getSize())));
            // Статус
            spotTable->setItem(row, 2, new QTableWidgetItem("Свободно"));
            // Номер авто (для свободных мест всегда "Нет")
            spotTable->setItem(row, 3, new QTableWidgetItem("Нет"));

            row++;
        }
    }
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
        QMessageBox::information(this, "Очередь", "Легковой автомобиль добавлен в очередь.");
    } else if (type == "Грузовой автомобиль") {
        QString cargoType = QInputDialog::getText(this, "Поставить в очередь", "Введите тип груза:", QLineEdit::Normal, "", &ok);
        if (!ok || cargoType.isEmpty()) return;

        auto truck = std::make_shared<Truck>(cargoType.toStdString(), licensePlate.toStdString());
        truckQueue.enqueue(truck);
        QMessageBox::information(this, "Очередь", "Грузовой автомобиль добавлен в очередь.");
    }
}

void MainWindow::parkVehicleFromQueue() {
    QStringList queues = {"Очередь легковых автомобилей", "Очередь грузовых автомобилей"};
    bool ok;

    QString queueType = QInputDialog::getItem(this, "Запарковать автомобиль", "Выберите очередь:", queues, 0, false, &ok);
    if (!ok || queueType.isEmpty()) return;

    int spotNumber = QInputDialog::getInt(this, "Запарковать автомобиль", "Введите номер парковочного места:", 1, 1, 10000, 1, &ok);
    if (!ok) return;

    try {
        std::shared_ptr<Vehicle> vehicle;

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

        std::string type = std::dynamic_pointer_cast<Car>(vehicle) ? "Car" : "Truck";
        std::string insertVehicleQuery = std::format(
            "INSERT INTO Vehicles (type, model, licensePlate, status) VALUES ('{}', '{}', '{}', 0);",
            type, vehicle->getModel(), vehicle->getLicensePlate()
        );

        if (!parkingLot->getDatabaseManager().executeQuery(insertVehicleQuery)) {
            throw std::runtime_error("Ошибка при добавлении автомобиля в базу данных.");
        }

        parkingLot->loadVehiclesFromDatabase();

        parkingLot->assignVehicleToSpot(vehicle->getLicensePlate(), spotNumber);

        QMessageBox::information(this, "Успех", QString("Автомобиль с номером %1 запаркован на месте %2.")
                                            .arg(QString::fromStdString(vehicle->getLicensePlate()))
                                            .arg(spotNumber));
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::onReturnToMenuClicked() {
    emit returnToLogin(); // Сигнал для возврата в LoginWindow
    close(); // Закрыть текущее окно
}

void MainWindow::displayParkingInfoAdmin() {
    // Очистка таблиц
    vehicleTable->setRowCount(0);
    spotTable->setRowCount(0);

    int vehicleRow = 0;
    int spotRow = 0;

    // Вывод автомобилей
    for (const auto &vehicle : parkingLot->getVehicles()) {
        vehicleTable->insertRow(vehicleRow);

        // Марка авто
        vehicleTable->setItem(vehicleRow, 0, new QTableWidgetItem(QString::fromStdString(vehicle->getModel())));
        // Регистрационный номер
        vehicleTable->setItem(vehicleRow, 1, new QTableWidgetItem(QString::fromStdString(vehicle->getLicensePlate())));
        // Статус
        vehicleTable->setItem(vehicleRow, 2, new QTableWidgetItem(vehicle->getStatus() ? "Запаркована" : "Не запаркована"));
        // Номер места
        QString spotNumber = "Нет";
        for (const auto &spot : parkingLot->getSpots()) {
            if (spot->getVehicle() && spot->getVehicle()->getLicensePlate() == vehicle->getLicensePlate()) {
                spotNumber = QString::number(spot->getNumber());
                break;
            }
        }
        vehicleTable->setItem(vehicleRow, 3, new QTableWidgetItem(spotNumber));

        vehicleRow++;
    }

    // Вывод парковочных мест
    double freePercentage = calculateFreeSpotPercentage(*parkingLot);

    // Добавляем строку с процентом свободных мест
    spotTable->insertRow(spotRow);
    spotTable->setItem(spotRow, 0, new QTableWidgetItem("Процент свободных мест:"));
    spotTable->setItem(spotRow, 1, new QTableWidgetItem(QString::number(freePercentage, 'f', 2) + " %"));
    spotTable->setSpan(spotRow, 1, 1, 3); // Объединяем ячейки для текста
    spotRow++;

    for (const auto &spot : parkingLot->getSpots()) {
        spotTable->insertRow(spotRow);

        // Номер места
        spotTable->setItem(spotRow, 0, new QTableWidgetItem(QString::number(spot->getNumber())));
        // Размер
        spotTable->setItem(spotRow, 1, new QTableWidgetItem(QString::fromStdString(spot->getSize())));
        // Статус
        spotTable->setItem(spotRow, 2, new QTableWidgetItem(spot->getStatus() ? "Занято" : "Свободно"));
        // Номер авто
        spotTable->setItem(spotRow, 3, new QTableWidgetItem(spot->getVehicle() ? QString::fromStdString(spot->getVehicle()->getLicensePlate()) : "Нет"));

        spotRow++;
    }
}

