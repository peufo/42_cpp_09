#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <iomanip>
#include <cmath>
#include <sys/time.h>

#define DEBUG(...) //__VA_ARGS__

class Pair
{
private:
    Pair();

public:
    int value;
    Pair *a;
    Pair *b;
    ~Pair();
    Pair(int value);
    Pair(Pair &a, Pair &b);
    Pair(Pair *a, Pair *b);
    Pair(const Pair &src);
    void swap();
    int getDeep();
    Pair &operator=(const Pair &src);
};

std::ostream &operator<<(std::ostream &os, Pair &pair);
std::ostream &operator<<(std::ostream &os, std::vector<Pair> &pairs);
std::ostream &operator<<(std::ostream &os, std::vector<Pair *> &pairs);
std::ostream &operator<<(std::ostream &os, std::vector<int> &values);

void mergeInsert(std::vector<int> &src);
void mergeInsert(std::deque<int> &src);

template <typename T, typename P>
void integersToPairs(T &src, P &pairs)
{
    for (typename T::iterator it = src.begin(); it != src.end(); it++)
        pairs.push_back(Pair(*it));
}

template <typename T, typename P>
void pairsToIntegers(P &pairs, T &result)
{
    for (typename P::iterator it = pairs.begin(); it != pairs.end(); it++)
        result.push_back(it->value);
}
template <typename T>
Pair *getPairNoNull(T &pairs)
{
    for (typename T::iterator it = pairs.begin(); it != pairs.end(); it++)
        if (*it)
            return *it;
    return NULL;
}

template <typename T>
void setSpecialOrder(int len, T &order)
{
    int power2 = 1;
    int groupSize = 0;
    int n = 0;

    while (n < len)
    {
        power2 *= 2;
        groupSize = power2 - groupSize;
        if (n + groupSize > len)
            groupSize = len - n;
        int index = n + groupSize - 1;
        while (index >= n)
        {
            order.push_back(index);
            index--;
        }
        n += groupSize;
    }
}

#endif
