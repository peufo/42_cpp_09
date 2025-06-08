#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <queue>
#include <algorithm>
#include <ctime>
#include "trim.hpp"

struct entrie
{
    std::tm date;
    std::time_t time;
    double amount;
};

entrie parseLine(const std::string &line)
{
    std::istringstream is(line);
    char sep1, sep2;
    std::string sep3;
    int year, month, day;
    float amount;
    is >> year >> sep1 >> month >> sep2 >> day;
    if (is.bad())
        throw std::invalid_argument("Parsing date failed");
    if (sep1 != '-' || sep2 != '-')
        throw std::invalid_argument("Incorrect date separator");
    if (month < 1 || month > 12)
        throw std::invalid_argument("Month is out of range 01-12");
    if (day < 1 || day > 31)
        throw std::invalid_argument("Day is out of range 01-31");
    is >> sep3;
    if (is.bad() || sep3 != "|")
        throw std::invalid_argument("Separator ` | ` not found");
    is >> amount;
    if (is.bad())
        throw std::invalid_argument("Parsing amount failed");
    if (amount < 0)
        throw std::invalid_argument("Not a positive amount");
    if (amount > 1000)
        throw std::invalid_argument("Too large amount");
    std::tm date = {};
    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;
    return (entrie){
        .date = date,
        .time = std::mktime(&date),
        .amount = amount};
}

int btc(char *filename)
{
    std::ifstream file(filename);
    if (!file.good())
    {
        std::cout << "Error: Cannot open file `" << filename << "`" << std::endl;
        return 1;
    }

    std::queue<entrie> entries;
    std::string line;
    int lineNo = 1;
    while (!file.eof())
    {
        getline(file, line);
        try
        {
            entries.push(parseLine(line));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in line " << lineNo << ": " << e.what() << '\n';
        }
        lineNo++;
    }

    for (; !entries.empty(); entries.pop())
    {
        entrie &e = entries.front();
        std::cout << "Print valid entrie " << e.amount << std::endl;
    }

    // 1. read file and build a list<entrie>
    // 2. sort list ?
    // 3. for each entries -> print move or error (Year-Month-Day, value(0 -> 1000))
    return 0;
}

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "Please, provide a file as an argument !" << std::endl;
        return 1;
    }
    return btc(av[1]);
}