#ifndef INVALIDTIMEFORMATERROR_HPP
#define INVALIDTIMEFORMATERROR_HPP

#include "ValidationError.hpp"

class InvalidTimeFormatError : public ValidationError {
public:
    explicit InvalidTimeFormatError(const std::string& message = "Неверный формат времени. Ожидаемый формат: YYYY-MM-DD HH:mm:ss")
        : ValidationError(message) {}
};

#endif // INVALIDTIMEFORMATERROR_HPP
