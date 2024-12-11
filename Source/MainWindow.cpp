#include "../Header/MainWindow.hpp"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QTextEdit>
#include <QInputDialog>
#include <QLineEdit>
#include <QHeaderView>
#include <QDateTime>
#include <QFileDialog>
#include <iostream>
#include <memory>
#include "../Header/Car.hpp"
#include "../Header/Truck.hpp" 
#include "../Header/Truck.hpp"
#include "../Header/InputValidator.hpp"
#include "../Header/ExportManager.hpp"
#include "../Header/FilterCriteria.hpp"

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

    
    vehicleTable = new QTableWidget(this);
    setupVehicleTable();
    layout->addWidget(new QLabel("Автомобили", this));
    layout->addWidget(vehicleTable);

    
    spotTable = new QTableWidget(this);
    setupSpotTable();
    layout->addWidget(new QLabel("Парковочные места", this));
    layout->addWidget(spotTable);

    auto *exportButton = new QPushButton("Скачать отчет", this);
    layout->addWidget(exportButton);
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::exportData);


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
    
    spotTable->setRowCount(0);
    vehicleTable->setRowCount(0);

    int row = 0;

    double freePercentage = calculateFreeSpotPercentage(*parkingLot);
    spotTable->insertRow(row);
    spotTable->setItem(row, 0, new QTableWidgetItem("Процент свободных мест:"));
    spotTable->setItem(row, 1, new QTableWidgetItem(QString::number(freePercentage, 'f', 2) + " %"));
    spotTable->setSpan(row, 1, 1, 3); 
    row++;


    for (const auto &spot : parkingLot->getSpots()) {
        if (!spot->getStatus()) { 
            spotTable->insertRow(row);

            spotTable->setItem(row, 0, new QTableWidgetItem(QString::number(spot->getNumber())));
            
            spotTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(spot->getSize())));
            
            spotTable->setItem(row, 2, new QTableWidgetItem("Свободно"));
            
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
    try {

        int number = InputValidator::getValidatedSpotNumber(this, 1, 10000);

        QStringList sizes = {"Легковое", "Грузовое"};

        bool ok;
        QString size = QInputDialog::getItem(
            this,
            "Добавить парковочное место",
            "Выберите размер места:",
            sizes,
            0,
            false,
            &ok
        );

        if (!ok || size.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Размер места не указан. Операция отменена.");
            return;
        }

        auto spot = std::make_shared<ParkingSpot>(number, size.toStdString(), false);
        parkingLot->addParkingSpot(spot, dbManager);

        QMessageBox::information(this, "Успех", "Парковочное место успешно добавлено.");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}




void MainWindow::removeParkingSpot() {
    bool ok;
    int spotNumber = QInputDialog::getInt(this, "Удалить парковочное место", 
                                          "Введите номер места:", 1, 1, 10000, 1, &ok);
    if (!ok) return;

    parkingLot->removeParkingSpot(spotNumber);
}


void MainWindow::assignVehicleToSpot() {
    try {
        
        QString licensePlate = InputValidator::getValidatedLicensePlate(this);

        int spotNumber = InputValidator::getValidatedSpotNumber(this, 1, 10000);

        QStringList options = {"Указать вручную", "Указать автоматически"};
        bool ok;
        QString choice = QInputDialog::getItem(this, "Ввод времени парковки", "Выберите способ указания времени:", options, 0, false, &ok);
        if (!ok) return;

        QString parkingTime;
        if (choice == "Указать вручную") {
            parkingTime = InputValidator::getValidatedTime(this);
            if (parkingTime.isEmpty()) {
                QMessageBox::warning(this, "Ошибка", "Неверный формат времени. Парковка отменена.");
                return;
            }
        } else {
            parkingTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        }

        parkingLot->assignVehicleToSpot(licensePlate.toStdString(), spotNumber, parkingTime.toStdString());
        QMessageBox::information(this, "Успех", "Автомобиль успешно запаркован.");
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}


void MainWindow::addVehicle() {
    QStringList types = {"Автомобиль", "Грузовик"};
    bool ok;
    QString type = QInputDialog::getItem(this, "Добавить транспортное средство", "Выберите тип:", types, 0, false, &ok);
    if (!ok || type.isEmpty()) return;

    try {
        QString licensePlate = InputValidator::getValidatedLicensePlate(this);

        if (InputValidator::isVehicleInMemory(licensePlate.toStdString(), parkingLot->getVehicles())) {
            QMessageBox::warning(this, "Ошибка", "Автомобиль с таким номерным знаком уже существует в памяти.");
            return;
        }

        if (parkingLot->getDatabaseManager().isVehicleInDatabase(licensePlate.toStdString())) {
            QMessageBox::warning(this, "Ошибка", "Автомобиль с таким номерным знаком уже существует в базе данных.");
            return;
        }

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
        QMessageBox::information(this, "Успех", "Транспортное средство успешно добавлено.");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}



void MainWindow::removeVehicle() {
    try {
        QString licensePlate = InputValidator::getValidatedLicensePlate(this);

        // Удаление транспортного средства
        parkingLot->removeVehicle(licensePlate.toStdString());

        QMessageBox::information(this, "Успех", "Транспортное средство успешно удалено. Связанное парковочное место освобождено.");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}



void MainWindow::releaseParkingSpot() {
    try {
        int spotNumber = InputValidator::getValidatedSpotNumber(this, 1, 10000);

        parkingLot->releaseParkingSpot(spotNumber);

        QMessageBox::information(this, "Успех", "Парковочное место успешно освобождено.");
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}

void MainWindow::sortAndSaveSpotsToDatabase() {
    parkingLot->sortAndSaveSpotsToDatabase();
}

void MainWindow::enqueueVehicle() {
    try {
        QStringList types = {"Легковой автомобиль", "Грузовой автомобиль"};
        bool ok;
        QString type = QInputDialog::getItem(this, "Поставить в очередь", "Выберите тип автомобиля:", types, 0, false, &ok);
        if (!ok || type.isEmpty()) return;

        QString licensePlate = InputValidator::getValidatedLicensePlate(this);

        if (type == "Легковой автомобиль") {
            QString model = QInputDialog::getText(this, "Поставить в очередь", "Введите модель автомобиля:", QLineEdit::Normal, "", &ok);
            if (!ok || model.isEmpty()) throw std::runtime_error("Модель автомобиля не может быть пустой.");

            auto car = std::make_shared<Car>(model.toStdString(), licensePlate.toStdString());
            carQueue.enqueue(car);
            QMessageBox::information(this, "Очередь", "Легковой автомобиль добавлен в очередь.");
        } else if (type == "Грузовой автомобиль") {
            QString cargoType = QInputDialog::getText(this, "Поставить в очередь", "Введите тип груза:", QLineEdit::Normal, "", &ok);
            if (!ok || cargoType.isEmpty()) throw std::runtime_error("Тип груза не может быть пустым.");

            auto truck = std::make_shared<Truck>(cargoType.toStdString(), licensePlate.toStdString());
            truckQueue.enqueue(truck);
            QMessageBox::information(this, "Очередь", "Грузовой автомобиль добавлен в очередь.");
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}


void MainWindow::parkVehicleFromQueue() {
    try {
        QStringList queues = {"Очередь легковых автомобилей", "Очередь грузовых автомобилей"};
        bool ok;
        QString queueType = QInputDialog::getItem(this, "Запарковать автомобиль", "Выберите очередь:", queues, 0, false, &ok);
        if (!ok || queueType.isEmpty()) return;

        
        int spotNumber = InputValidator::getValidatedSpotNumber(this, 1, 10000);

        
        std::shared_ptr<Vehicle> vehicle;
        if (queueType == "Очередь легковых автомобилей") {
            if (carQueue.isEmpty()) {
                throw std::runtime_error("Очередь легковых автомобилей пуста.");
            }
            vehicle = carQueue.dequeue();
        } else if (queueType == "Очередь грузовых автомобилей") {
            if (truckQueue.isEmpty()) {
                throw std::runtime_error("Очередь грузовых автомобилей пуста.");
            }
            vehicle = truckQueue.dequeue();
        }

        
        if (parkingLot->getDatabaseManager().isVehicleInDatabase(vehicle->getLicensePlate())) {
            throw std::runtime_error("Автомобиль с таким номерным знаком уже существует в базе данных.");
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


        QString parkingTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        parkingLot->assignVehicleToSpot(vehicle->getLicensePlate(), spotNumber, parkingTime.toStdString());

        QMessageBox::information(this, "Успех", QString("Автомобиль с номером %1 запаркован на месте %2.")
                                            .arg(QString::fromStdString(vehicle->getLicensePlate()))
                                            .arg(spotNumber));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}

void MainWindow::onReturnToMenuClicked() {
    emit returnToLogin(); 
    close(); 
}

void MainWindow::displayParkingInfoAdmin() {

    vehicleTable->setRowCount(0);
    spotTable->setRowCount(0);

    int vehicleRow = 0;
    int spotRow = 0;


    for (const auto &vehicle : parkingLot->getVehicles()) {
        vehicleTable->insertRow(vehicleRow);

        vehicleTable->setItem(vehicleRow, 0, new QTableWidgetItem(QString::fromStdString(vehicle->getModel())));

        vehicleTable->setItem(vehicleRow, 1, new QTableWidgetItem(QString::fromStdString(vehicle->getLicensePlate())));
 
        vehicleTable->setItem(vehicleRow, 2, new QTableWidgetItem(vehicle->getStatus() ? "Запаркована" : "Не запаркована"));

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

    double freePercentage = calculateFreeSpotPercentage(*parkingLot);

    spotTable->insertRow(spotRow);
    spotTable->setItem(spotRow, 0, new QTableWidgetItem("Процент свободных мест:"));
    spotTable->setItem(spotRow, 1, new QTableWidgetItem(QString::number(freePercentage, 'f', 2) + " %"));
    spotTable->setSpan(spotRow, 1, 1, 3); 
    spotRow++;

    for (const auto &spot : parkingLot->getSpots()) {
        spotTable->insertRow(spotRow);

        spotTable->setItem(spotRow, 0, new QTableWidgetItem(QString::number(spot->getNumber())));

        spotTable->setItem(spotRow, 1, new QTableWidgetItem(QString::fromStdString(spot->getSize())));

        spotTable->setItem(spotRow, 2, new QTableWidgetItem(spot->getStatus() ? "Занято" : "Свободно"));

        spotTable->setItem(spotRow, 3, new QTableWidgetItem(spot->getVehicle() ? QString::fromStdString(spot->getVehicle()->getLicensePlate()) : "Нет"));

        spotRow++;
    }
}


void MainWindow::exportData() {
    QStringList formats = {"JSON", "XML", "XLSX", "DOCX"};
    bool ok;
    QString format = QInputDialog::getItem(this, "Выберите формат", "Формат файла:", formats, 0, false, &ok);

    if (!ok || format.isEmpty()) return;

    FilterCriteria criteria = getFilterCriteriaFromUI();

    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить файл", "",
        format == "JSON" ? "JSON files (*.json)" :
        format == "XML" ? "XML files (*.xml)" :
        format == "XLSX" ? "Excel files (*.xlsx)" :
        format == "DOCX" ? "Word files (*.docx)" : "All files (*.*)");

    if (filePath.isEmpty()) return;

    try {
        if (criteria.type == "Автомобили") {
            exportManager.exportVehicles(format, filePath, parkingLot->getVehicles(), criteria);
        } else if (criteria.type == "Парковочные места") {
            exportManager.exportParkingSpots(format, filePath, parkingLot->getSpots(), criteria);
        }
        QMessageBox::information(this, "Успех", "Отчет успешно сохранен.");
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}

FilterCriteria MainWindow::getFilterCriteriaFromUI() {
    // Реализуйте интерфейс для выбора фильтров (например, через QInputDialog или отдельное окно)
    FilterCriteria criteria;
    criteria.type = QInputDialog::getItem(this, "Выбор данных", "Что экспортировать:", {"Автомобили", "Парковочные места"});
    if (criteria.type == "Автомобили") {
        criteria.includeFreeVehicles = QMessageBox::question(this, "Фильтр", "Включить свободные автомобили?") == QMessageBox::Yes;
        criteria.includeParkedVehicles = QMessageBox::question(this, "Фильтр", "Включить запаркованные автомобили?") == QMessageBox::Yes;
    } else if (criteria.type == "Парковочные места") {
        criteria.isOccupied = QMessageBox::question(this, "Фильтр", "Только занятые места?") == QMessageBox::Yes;
        criteria.numberFrom = QInputDialog::getInt(this, "Фильтр", "Номер места от:", 1, 1, 10000);
        criteria.numberTo = QInputDialog::getInt(this, "Фильтр", "Номер места до:", 1, 1, 10000);
        criteria.size = QInputDialog::getText(this, "Фильтр", "Размер парковочного места:");
    }
    return criteria;
}


void MainWindow::setupUI() {
    auto *layout = new QVBoxLayout;

    auto *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Подключаем кнопку к слоту
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::exportData);
}

