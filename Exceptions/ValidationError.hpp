#ifndef VALIDATIONERROR_HPP
#define VALIDATIONERROR_HPP

#include <stdexcept>
#include <string>

class ValidationError : public std::runtime_error {
public:
    explicit ValidationError(const std::string& message)
        : std::runtime_error(message) {}
};

#endif // VALIDATIONERROR_HPP