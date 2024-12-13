#ifndef DUPLICATEVEHICLEERROR_HPP
#define DUPLICATEVEHICLEERROR_HPP

#include "ValidationError.hpp"

class DuplicateVehicleError : public ValidationError {
public:
    explicit DuplicateVehicleError(const std::string& message = "Транспортное средство с таким номерным знаком уже существует.")
        : ValidationError(message) {}
};

#endif // DUPLICATEVEHICLEERROR_HPP
