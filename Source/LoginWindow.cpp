#include "../Header/LoginWindow.hpp"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QHeaderView>
#include <QInputDialog> 
#include <QLineEdit>    

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent), dbManager("../parking_lot.db") {
    
    parkingLot = std::make_unique<ParkingLot>("Центральная парковка", dbManager);

    setWindowTitle("Вход в систему");
    setFixedSize(800, 600);

    auto *layout = new QVBoxLayout(this);

    auto *label = new QLabel("Выберите действие:", this);
    layout->addWidget(label);

    adminButton = new QPushButton("Войти в систему как админ", this);
    userButton = new QPushButton("Отобразить парковочные места", this);

    layout->addWidget(adminButton);
    layout->addWidget(userButton);

   
    infoTable = new QTableWidget(this);
    infoTable->hide();
    layout->addWidget(infoTable);

    setupTable(); 


    connect(adminButton, &QPushButton::clicked, this, &LoginWindow::openAdminPanel);
    connect(userButton, &QPushButton::clicked, this, &LoginWindow::openUserView);
}

void LoginWindow::setupTable() {
    infoTable->setColumnCount(4);
    infoTable->setHorizontalHeaderLabels({"Номер места", "Размер", "Статус", "Транспорт"});
    infoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); 
}

void LoginWindow::openAdminPanel() {
    bool ok;
    QString password = QInputDialog::getText(this, "Вход для администратора",
                                             "Введите пароль:",
                                             QLineEdit::Password, 
                                             "", &ok);
    if (ok && !password.isEmpty()) {
        if (password == adminPassword) {
            auto *mainWindow = new MainWindow();
            mainWindow->setWindowTitle("Панель администратора - Автопарковка");
            mainWindow->resize(600, 800);

            connect(mainWindow, &MainWindow::returnToLogin, this, &LoginWindow::show);

            mainWindow->show();
            hide(); 
        } else {
            QMessageBox::warning(this, "Ошибка", "Неверный пароль!");
        }
    }
}

void LoginWindow::openUserView() {

    infoTable->setRowCount(0);

    int row = 0;

    double freePercentage = calculateFreeSpotPercentage(*parkingLot);
    infoTable->insertRow(row);
    infoTable->setItem(row, 0, new QTableWidgetItem("Процент свободных мест:"));
    infoTable->setItem(row, 1, new QTableWidgetItem(QString::number(freePercentage, 'f', 2) + " %"));
    infoTable->setSpan(row, 1, 1, 3); 
    row++;

    for (const auto &spot : parkingLot->getSpots()) {
        if (!spot->getStatus()) { 
            infoTable->insertRow(row);

            infoTable->setItem(row, 0, new QTableWidgetItem(QString::number(spot->getNumber())));
            infoTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(spot->getSize())));
            infoTable->setItem(row, 2, new QTableWidgetItem("Свободно"));
            infoTable->setItem(row, 3, new QTableWidgetItem("Нет"));

            row++;
        }
    }

    adminButton->hide();
    userButton->hide();
    infoTable->show();

    auto *returnButton = new QPushButton("Вернуться в начальное меню", this);
    returnButton->setObjectName("returnButton");
    layout()->addWidget(returnButton);

    connect(returnButton, &QPushButton::clicked, this, &LoginWindow::returnToMenu);
}

void LoginWindow::returnToMenu() {

    infoTable->setRowCount(0);
    infoTable->hide();

    auto *returnButton = findChild<QPushButton *>("returnButton");
    if (returnButton) {
        layout()->removeWidget(returnButton);
        delete returnButton;
    }

    adminButton->show();
    userButton->show();
}

