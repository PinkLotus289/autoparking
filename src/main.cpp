#include "Parking.h"
#include <iostream>

int main() {
    Parking parking(5);

    parking.addCar("A123BC", "Toyota");
    parking.addCar("B456DE", "Honda");

    std::cout << "Available spots: " << parking.availableSpots() << std::endl;
    
    parking.listCars();

    parking.updateCar("A123BC", "A321CB", "Mazda");
    
    parking.listCars();

    parking.removeCar("B456DE");
    
    parking.listCars();

    return 0;
}
