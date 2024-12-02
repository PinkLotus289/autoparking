#ifndef LOGINWINDOW_HPP
#define LOGINWINDOW_HPP

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QTableWidget>
#include <memory>
#include "ParkingLot.hpp"
#include "DatabaseManager.hpp"
#include "MainWindow.hpp" 

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

private slots:
    void openAdminPanel();
    void openUserView(); // Обновлённая функция для отображения парковочных мест
    void returnToMenu();

private:
    QPushButton *adminButton;
    QPushButton *userButton;
    QTextEdit *infoDisplay;
    QTableWidget *infoTable;

    std::unique_ptr<ParkingLot> parkingLot; // Хранение объекта парковки
    DatabaseManager dbManager;              // Менеджер базы данных

    const QString adminPassword = "admin123";

    void setupTable();
};

#endif

