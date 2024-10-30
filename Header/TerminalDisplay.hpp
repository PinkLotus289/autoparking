
#ifndef TERMINALDISPLAY_HPP
#define TERMINALDISPLAY_HPP

#include <string>

class TerminalDisplay {
public:
    virtual ~TerminalDisplay() = default;

    virtual void display() = 0; 
};

#endif 