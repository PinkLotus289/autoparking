#ifndef INVALIDLICENSEPLATEERROR_HPP
#define INVALIDLICENSEPLATEERROR_HPP

#include "ValidationError.hpp"

class InvalidLicensePlateError : public ValidationError {
public:
    explicit InvalidLicensePlateError(const std::string& message = "Неверный формат номерного знака. Ожидаемый формат: 1234ГС-5")
        : ValidationError(message) {}
};

#endif // INVALIDLICENSEPLATEERROR_HPP
