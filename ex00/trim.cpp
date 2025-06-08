#include "trim.hpp"

static bool isSpace(unsigned char c) { return std::isspace(c); }
static bool isNotSpace(unsigned char c) { return !std::isspace(c); }

std::string trim(std::string s)
{
    std::string::iterator start = std::find_if(s.begin(), s.end(), isNotSpace);
    std::string::iterator end = std::find_if(start + 1, s.end(), isSpace);
    s.erase(s.begin(), start);
    s.erase(end, s.end());
    return s;
}
