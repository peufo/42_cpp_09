#include "BitcoinExchange.hpp"
#include <iostream>
#include <limits>

double findMarketPrice(const BitcoinExchange &market, time_t time)
{
    for (BitcoinExchange::const_iterator i = market.end(); i != market.begin(); i--)
    {
        if (i->time <= time)
            return i->amount;
    }
    return 0;
}

void btc(char *filename)
{
    BitcoinExchange market;
    BitcoinExchange inputs(filename, '|', 1000);

    char date[32];
    for (BitcoinExchange::const_iterator i = inputs.begin(); i != inputs.end(); i++)
    {
        std::strftime(date, 32, "%d %B %Y", &i->date);
        std::cout
            << date
            << " => "
            << i->amount
            << " = "
            << i->amount * market.findEntrieBefore(*i).amount
            << std::endl;
    }
}

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "Please, provide a file as an argument !" << std::endl;
        return 1;
    }
    btc(av[1]);
    return 0;
}