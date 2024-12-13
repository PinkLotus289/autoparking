#ifndef INVALIDSPOTNUMBERERROR_HPP
#define INVALIDSPOTNUMBERERROR_HPP

#include "ValidationError.hpp"

class InvalidSpotNumberError : public ValidationError {
public:
    explicit InvalidSpotNumberError(const std::string& message = "Номер парковочного места вне допустимого диапазона.")
        : ValidationError(message) {}
};

#endif // INVALIDSPOTNUMBERERROR_HPP
