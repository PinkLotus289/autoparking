#include "../Header/LoginWindow.hpp"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QHeaderView>
#include <QInputDialog> // Для QInputDialog
#include <QLineEdit>    // Для QLineEdit

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent), dbManager("../parking_lot.db") {
    
    parkingLot = std::make_unique<ParkingLot>("Центральная парковка", dbManager);

    setWindowTitle("Вход в систему");
    setFixedSize(600, 400);

    auto *layout = new QVBoxLayout(this);

    auto *label = new QLabel("Выберите действие:", this);
    layout->addWidget(label);

    adminButton = new QPushButton("Войти в систему как админ", this);
    userButton = new QPushButton("Отобразить парковочные места", this);

    layout->addWidget(adminButton);
    layout->addWidget(userButton);

    // Таблица для отображения информации
    infoTable = new QTableWidget(this);
    infoTable->hide(); // Скрыто по умолчанию
    layout->addWidget(infoTable);

    setupTable(); // Настройка таблицы

    // Связывание кнопок с действиями
    connect(adminButton, &QPushButton::clicked, this, &LoginWindow::openAdminPanel);
    connect(userButton, &QPushButton::clicked, this, &LoginWindow::openUserView);
}

void LoginWindow::setupTable() {
    infoTable->setColumnCount(4);
    infoTable->setHorizontalHeaderLabels({"Номер места", "Размер", "Статус", "Транспорт"});
    infoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Растягивать столбцы
}

void LoginWindow::openAdminPanel() {
    bool ok;
    QString password = QInputDialog::getText(this, "Вход для администратора",
                                             "Введите пароль:",
                                             QLineEdit::Password, // Указание типа ввода
                                             "", &ok);
    if (ok && !password.isEmpty()) {
        if (password == adminPassword) {
            auto *mainWindow = new MainWindow();
            mainWindow->setWindowTitle("Панель администратора - Автопарковка");
            mainWindow->resize(600, 800);

            connect(mainWindow, &MainWindow::returnToLogin, this, &LoginWindow::show);

            mainWindow->show();
            hide(); // Скрываем LoginWindow, но не закрываем его
        } else {
            QMessageBox::warning(this, "Ошибка", "Неверный пароль!");
        }
    }
}

void LoginWindow::openUserView() {
    /*
    infoTable->setRowCount(0); // Очистить таблицу перед заполнением

    int row = 0;

    double freePercentage = calculateFreeSpotPercentage(*parkingLot);
    infoTable->insertRow(row);
    infoTable->setItem(row, 0, new QTableWidgetItem("Процент свободных мест:"));
    infoTable->setItem(row, 1, new QTableWidgetItem(QString::number(freePercentage, 'f', 2) + " %"));
    infoTable->setSpan(row, 1, 1, 3); // Объединяем ячейки для текста
    row++;

    for (const auto &spot : parkingLot->getSpots()) {
        infoTable->insertRow(row);

        infoTable->setItem(row, 0, new QTableWidgetItem(QString::number(spot->getNumber())));
        infoTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(spot->getSize())));
        infoTable->setItem(row, 2, new QTableWidgetItem(spot->getStatus() ? "Занято" : "Свободно"));
        infoTable->setItem(row, 3, new QTableWidgetItem(spot->getVehicle() ? 
                                                        QString::fromStdString(spot->getVehicle()->getLicensePlate()) : 
                                                        "Нет"));
        row++;
    }
    */

    // Очистка таблицы парковочных мест
    infoTable->setRowCount(0);

    int row = 0;

    // Сначала добавляем строку с процентом свободных мест
    double freePercentage = calculateFreeSpotPercentage(*parkingLot);
    infoTable->insertRow(row);
    infoTable->setItem(row, 0, new QTableWidgetItem("Процент свободных мест:"));
    infoTable->setItem(row, 1, new QTableWidgetItem(QString::number(freePercentage, 'f', 2) + " %"));
    infoTable->setSpan(row, 1, 1, 3); // Объединяем ячейки для текста
    row++;

    // Затем выводим список свободных мест
    for (const auto &spot : parkingLot->getSpots()) {
        if (!spot->getStatus()) { // Условие: только свободные места
            infoTable->insertRow(row);

            // Номер места
            infoTable->setItem(row, 0, new QTableWidgetItem(QString::number(spot->getNumber())));
            // Размер
            infoTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(spot->getSize())));
            // Статус
            infoTable->setItem(row, 2, new QTableWidgetItem("Свободно"));
            // Номер авто (для свободных мест всегда "Нет")
            infoTable->setItem(row, 3, new QTableWidgetItem("Нет"));

            row++;
        }
    }

    // Показать таблицу и кнопку возврата, скрыть остальные кнопки
    adminButton->hide();
    userButton->hide();
    infoTable->show();

    auto *returnButton = new QPushButton("Вернуться в начальное меню", this);
    returnButton->setObjectName("returnButton");
    layout()->addWidget(returnButton);

    connect(returnButton, &QPushButton::clicked, this, &LoginWindow::returnToMenu);
}

void LoginWindow::returnToMenu() {
    // Очистить таблицу
    infoTable->setRowCount(0);
    infoTable->hide();

    // Удалить кнопку возврата, если она есть
    auto *returnButton = findChild<QPushButton *>("returnButton");
    if (returnButton) {
        layout()->removeWidget(returnButton);
        delete returnButton;
    }

    // Показать кнопки для выбора действий
    adminButton->show();
    userButton->show();
}

