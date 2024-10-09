#ifndef PARKINGSPOT_HPP
#define PARKINGSPOT_HPP

#include <string>

class ParkingSpot {
public:
    ParkingSpot(int number, std::string_view size, bool occupied = false);

    int getNumber() const;
    void displayParkingSpot(bool isAdmin) const;

private:
    int number;
    std::string size;
    bool occupied;
};

#endif // PARKINGSPOT_HPP
