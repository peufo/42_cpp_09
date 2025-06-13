#include "RPN.hpp"

void RPN::handleValue(std::string &value)
{
    if (value.length() != 1)
        throw std::invalid_argument(std::string("More than one character: `") + value + "`");
    char c = value[0];
    if (c == '+')
        return this->push(this->takeTop() + this->takeTop());
    if (c == '-')
    {
        double b = this->takeTop();
        double a = this->takeTop();
        return this->push(a - b);
    }
    if (c == '*')
        return this->push(this->takeTop() * this->takeTop());
    if (c == '/')
    {
        double b = this->takeTop();
        double a = this->takeTop();
        return this->push(a / b);
    }

    if (!std::isdigit(c))
        throw std::invalid_argument(std::string("Invalid character: `") + c + "`");
    this->push(c - '0');
}

double RPN::takeTop()
{
    if (!this->size())
        throw std::invalid_argument("Missing operand");
    double value = this->top();
    this->pop();
    return value;
}

double RPN::compute(const std::string &str)
{
    std::istringstream is(str);
    std::string value;

    while (!is.eof())
    {
        is >> value;
        if (!is.fail())
            this->handleValue(value);
    }

    if (this->empty())
        throw std::invalid_argument("No result!");
    if (this->size() > 1)
        throw std::invalid_argument("Missing operation");
    return this->takeTop();
}
