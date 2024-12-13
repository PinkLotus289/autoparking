#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <QPushButton>

#include <QDialog>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>

#include <memory>
#include "ParkingLot.hpp"
#include "DatabaseManager.hpp"
#include "Queue.hpp"
#include "Car.hpp"
#include "Truck.hpp"
#include "FilterCriteria.hpp"
#include "ExportManager.hpp"

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
    void exportData();
    FilterCriteria getFilterCriteriaFromUI();

private:
    QTableWidget *vehicleTable;  
    QTableWidget *spotTable;     
    sqlite3 *db;
    DatabaseManager dbManager;
    std::unique_ptr<ParkingLot> parkingLot;
    

    QString generateParkingInfo(bool isAdmin);

    Queue<std::shared_ptr<Car>> carQueue;
    Queue<std::shared_ptr<Truck>> truckQueue;

    void setupVehicleTable(); 
    void setupSpotTable();   

    QPushButton *exportButton; // Кнопка для экспорта отчета
    ExportManager exportManager; // Объект менеджера экспорта

    void setupUI(); // Функция для настройки интерфейса
};

#endif 



