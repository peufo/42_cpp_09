#include "DataBase.hpp"
#include <iostream>
#include <limits>
#include <algorithm>

double findMarketPrice(const DataBase &market, time_t time)
{
    for (DataBase::const_iterator i = market.end(); i != market.begin(); i--)
    {
        if (i->time <= time)
            return i->amount;
    }
    return 0;
}

void btc(char *filename)
{
    DataBase market("data.csv", ',', std::numeric_limits<int>::max());
    DataBase inputs(filename, '|', 1000);

    char date[32];
    for (DataBase::const_iterator i = inputs.begin(); i != inputs.end(); i++)
    {
        std::strftime(date, 32, "%d %B %Y", &i->date);
        double marketPrice = findMarketPrice(market, i->time);
        std::cout << date << " => " << i->amount << " = " << i->amount * marketPrice << std::endl;
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