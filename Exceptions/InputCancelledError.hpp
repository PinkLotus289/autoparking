#ifndef INPUTCANCELLEDERROR_HPP
#define INPUTCANCELLEDERROR_HPP

#include "ValidationError.hpp"

class InputCancelledError : public ValidationError {
public:
    explicit InputCancelledError(const std::string& message = "Ввод был отменен пользователем.")
        : ValidationError(message) {}
};

#endif 
