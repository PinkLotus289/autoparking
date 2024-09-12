#ifndef CAR_H
#define CAR_H

class Car {
private:
    char* licensePlate;
    char* model;

public:
    Car(const char* plate, const char* mod);
    ~Car();

    const char* getLicensePlate() const;
    const char* getModel() const;

    void setLicensePlate(const char* plate);
    void setModel(const char* mod);

    void displayCarInfo() const;
};

#endif // CAR_H

