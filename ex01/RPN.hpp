#ifndef RPN_HPP
#define RPN_HPP

#include <string>
#include <stack>
#include <sstream>
#include <iostream>

class RPN : std::stack<double>
{
private:
    void handleValue(std::string &value);
    double takeTop();

public:
    double compute(const std::string &str);
};

#endif
