#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <ctime>

struct entrie_t
{
    std::tm date;
    std::time_t time;
    double amount;
};

class DataBase : public std::list<entrie_t>
{

private:
    const char *filename;
    char separator;
    int maxAmount;
    entrie_t parseLine(const std::string &line);
    void loadFile();

public:
    DataBase(const char *filename, char separator, int maxAmount);
};

#endif
