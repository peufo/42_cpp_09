#include "PmergeMe.hpp"

std::ostream &operator<<(std::ostream &os, Slice &slice)
{
    os << "[" << std::setw(2) << *slice.begin;
    for (std::vector<int>::iterator it = slice.begin + 1; it != slice.end; it++)
    {
        os << ", " << std::setw(2) << *it;
    }
    return os << "]";
}

std::ostream &operator<<(std::ostream &os, std::vector<Slice> &s)
{
    for (std::vector<Slice>::iterator it = s.begin(); it != s.end(); it++)
    {
        os << *it << '\n';
    }
    return os;
}

// ================= class Pair ==========================

Slice::~Slice() {}

Slice::Slice()
{
}

Slice::Slice(std::vector<int>::iterator &begin, std::vector<int>::iterator &end)
    : begin(begin),
      end(end)
{
}

Slice::Slice(std::vector<int>::iterator &it) : begin(it), end(it + 1)
{
}

Slice::Slice(const Slice &src) : begin(src.begin), end(src.end)
{
}

Slice &Slice::operator=(const Slice &src)
{
    if (this == &src)
        return *this;
    this->begin = src.begin;
    this->end = src.end;
    return *this;
}

// ================= class Pair ==========================

Pair::~Pair()
{
}

Pair::Pair(int value) : value(value), a(NULL), b(NULL)
{
}

Pair::Pair(Pair &b) : value(b.value), a(NULL), b(&b)
{
}

Pair::Pair(Pair &a, Pair &b) : value(b.value), a(&a), b(&b)
{
    if (a.value > b.value)
        this->swap();
}

Pair::Pair() : value(0), a(NULL), b(NULL)
{
}

Pair::Pair(const Pair &src) : value(src.value), a(src.a), b(src.b)
{
}

void Pair::swap()
{
    Pair *tmp = this->a;
    this->a = this->b;
    this->b = tmp;
}

Pair &Pair::operator=(const Pair &src)
{
    if (this == &src)
        return *this;
    this->value = src.value;
    this->a = src.a;
    this->b = src.b;
    return *this;
}

std::vector<Slice>::iterator findInsertionPoint(std::vector<Slice> &slices, std::vector<Slice>::iterator &toInsert)
{
    std::vector<Slice>::iterator left = slices.begin();
    std::vector<Slice>::iterator right = slices.end();

    while (left < right)
    {
        size_t size = right - left;
        std::vector<Slice>::iterator it = left + size / 2;
        if (*(it->end - 1) < *(toInsert->end - 1))
            left = it;
        else
            right = it - 1;
    }
    return left;
}

void mergeInsert(std::vector<Slice> &slices)
{

    std::cout << "slices before:\n"
              << slices << std::endl;

    std::vector<Slice>::iterator last = slices.size() & 1 ? slices.end() - 1 : slices.end();
    std::vector<Slice> paires;

    for (std::vector<Slice>::iterator it = slices.begin(); it != last; it += 2)
    {
        std::vector<Slice>::iterator next = it + 1;
        if (*(it->end - 1) > *(next->end - 1))
            std::swap_ranges(it->begin, it->end, next->begin);
        paires.push_back(Slice(it->begin, next->end));
    }

    std::cout << "paired:\n"
              << paires << std::endl;
    if (paires.size() < 2)
        return;

    mergeInsert(paires);
    std::cout << "slices after:\n"
              << paires << std::endl;

    std::vector<Slice> toInsert;
    for (std::vector<Slice>::iterator it = slices.begin() + 2; it != last; it += 2)
    {
        toInsert.push_back(*it);
        slices.erase(it);
    }

    // TODO: use determined order of insertion
    // for (std::vector<Slice>::iterator it = toInsert.begin(); it != toInsert.end(); it++)
    // {
    //     std::vector<Slice>::iterator insertionPoint = findInsertionPoint(slices, it);

    // }
}
