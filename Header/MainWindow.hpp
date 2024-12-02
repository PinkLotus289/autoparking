#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <memory>
#include "ParkingLot.hpp"
#include "DatabaseManager.hpp"
#include "Queue.hpp"
#include "Car.hpp"
#include "Truck.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void displayParkingInfoUser();
    void displayParkingInfoAdmin();

signals:
    void returnToLogin();

private slots:
    void removeVehicle(); 
    void removeParkingSpot();
    void addVehicle(); 
    void addParkingSpot();
    void assignVehicleToSpot(); 
    void releaseParkingSpot();
    void sortAndSaveSpotsToDatabase();
    void enqueueVehicle();
    void parkVehicleFromQueue();
    void onReturnToMenuClicked();

private:
    QTableWidget *vehicleTable;  // Таблица для автомобилей
    QTableWidget *spotTable;     // Таблица для парковочных мест
    sqlite3 *db;
    DatabaseManager dbManager;
    std::unique_ptr<ParkingLot> parkingLot;
    

    QString generateParkingInfo(bool isAdmin);

    Queue<std::shared_ptr<Car>> carQueue;
    Queue<std::shared_ptr<Truck>> truckQueue;

    void setupVehicleTable(); // Настройка таблицы для автомобилей
    void setupSpotTable();    // Настройка таблицы для парковочных мест
};

#endif 

