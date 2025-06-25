#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cmath>

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

void mergeInsert(std::vector<int> &src);

std::ostream &operator<<(std::ostream &os, Pair &pair);
// TODO: templates ?
std::ostream &operator<<(std::ostream &os, std::vector<int> &values);
std::ostream &operator<<(std::ostream &os, std::vector<Pair *> &pairs);
std::ostream &operator<<(std::ostream &os, std::vector<Pair> &pairs);

// Utils
void readValues(std::vector<int> &src, std::vector<Pair> &pairs);
void writeResult(std::vector<Pair> &sorted, std::vector<int> &result);

#endif
