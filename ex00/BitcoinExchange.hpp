#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <ctime>
#include <limits>

struct entrie_t
{
    std::tm date;
    std::time_t time;
    double amount;
};

class BitcoinExchange : public std::list<entrie_t>
{

private:
    const char *filename;
    char separator;
    int maxAmount;
    void parseLine(const std::string &line);
    void loadFile();

public:
    BitcoinExchange();
    BitcoinExchange(const char *filename, char separator, int maxAmount);
    BitcoinExchange(const BitcoinExchange &src);
    BitcoinExchange &operator=(const BitcoinExchange &src);
    ~BitcoinExchange();
    const entrie_t &findEntrieBefore(const entrie_t &entrie) const;
};

#endif
