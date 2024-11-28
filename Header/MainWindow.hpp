#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
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

private slots:
    void displayParkingInfoUser();
    void displayParkingInfoAdmin();
    void removeVehicle(); 
    void removeParkingSpot();
    void addVehicle(); 
    void addParkingSpot();
    void assignVehicleToSpot(); 
    void releaseParkingSpot();
    void sortAndSaveSpotsToDatabase();
    void enqueueVehicle();
    void parkVehicleFromQueue();

private:
    QTextEdit *infoDisplay;
    sqlite3 *db;
    DatabaseManager dbManager;
    std::unique_ptr<ParkingLot> parkingLot;

    QString generateParkingInfo(bool isAdmin);

    Queue<std::shared_ptr<Car>> carQueue;
    Queue<std::shared_ptr<Truck>> truckQueue;
};

#endif 

