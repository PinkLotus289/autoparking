// Entity.hpp
#ifndef GET_HPP
#define GET_HPP

#include <string>

class GetField {
public:
    virtual ~GetField() = default;

    virtual bool getStatus() const = 0;

};

#endif  