#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cmath>

class Slice
{

private:
    Slice();

public:
    std::vector<int>::iterator begin;
    std::vector<int>::iterator end;
    ~Slice();
    Slice(std::vector<int>::iterator &begin, std::vector<int>::iterator &end);
    Slice(std::vector<int>::iterator &it);
    Slice(const Slice &src);
    Slice &operator=(const Slice &src);
};

class Pair
{
private:
    int value;
    Pair *a;
    Pair *b;
    Pair();

public:
    ~Pair();
    Pair(int value);
    Pair(Pair &a, Pair &b);
    Pair(Pair &b);
    Pair(const Pair &src);
    void swap();
    Pair &operator=(const Pair &src);
};

std::ostream &operator<<(std::ostream &os, Slice &slice);
std::ostream &operator<<(std::ostream &os, std::vector<Slice> &s);

void mergeInsert(std::vector<Slice> &s);

#endif
