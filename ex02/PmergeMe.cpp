#include "PmergeMe.hpp"

static std::ostream &writeN(std::ostream &os, const std::string &value, int n)
{
    for (int i = 0; i < n; i++)
        os << value;
    return os;
}

std::ostream &operator<<(std::ostream &os, Pair &pair)
{
    int deep = pair.getDeep();
    int padding = 1;
    for (int i = 0; i < deep; i++)
        padding = padding * 2 + 1;
    os << std::setw(padding + 1) << pair.value << '\n';
    std::vector<Pair *> pairs;
    pairs.push_back(pair.a);
    pairs.push_back(pair.b);
    os << pairs;
    return os;
}

std::ostream &operator<<(std::ostream &os, std::vector<int> &values)
{
    if (!values.size())
    {
        os << "[empty array]" << std::endl;
        return os;
    }
    os << "[" << values[0];
    for (std::vector<int>::iterator it = values.begin() + 1; it != values.end(); it++)
        os << " " << *it;
    os << "]" << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, std::vector<Pair *> &pairs)
{
    std::vector<Pair *> children;
    if (pairs.size() == 0 || !pairs[1])
        return os;
    int deep = pairs[1]->getDeep();
    int padding = 1;
    for (int i = 0; i < deep; i++)
        padding = padding * 2 + 1;
    int padding2 = padding * 2 + 1;

    writeN(os, " ", padding);
    for (std::vector<Pair *>::iterator it = pairs.begin(); it != pairs.end(); it += 2)
    {
        os << "╭";
        writeN(os, "─", padding) << "┴";
        writeN(os, "─", padding) << "╮";
        if (it != pairs.end() - 2)
            writeN(os, " ", padding2);
    }
    os << '\n';
    for (std::vector<Pair *>::iterator it = pairs.begin(); it != pairs.end(); it++)
    {
        if (*it)
        {
            os << std::setw(padding + 1) << (*it)->value;
            children.push_back((*it)->a);
            children.push_back((*it)->b);
        }
        else
            os << std::setw(padding + 1) << 'x';
        if (it != pairs.end() - 1)
            writeN(os, " ", padding + 1);
    }
    os << '\n';
    if (children.size())
        os << children;
    return os;
}

Pair::~Pair()
{
}

Pair::Pair(int value) : value(value), a(NULL), b(NULL)
{
}

Pair::Pair(Pair &a, Pair &b) : value(b.value), a(&a), b(&b)
{
    if (this->a->value > this->b->value)
        this->swap();
}

Pair::Pair(Pair *a, Pair *b) : value(b->value), a(a), b(b)
{
    if (this->a && this->b && this->a->value > this->b->value)
        this->swap();
    if (this->a && !this->b)
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
    this->value = this->b->value;
}

int Pair::getDeep()
{
    int deep = 0;
    Pair *p = this->b;
    while (p)
    {
        deep++;
        p = p->b;
    }
    return deep;
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

std::vector<Pair>::iterator findInsertionPoint(std::vector<Pair> &pairs, std::vector<Pair>::iterator &toInsert)
{
    std::vector<Pair>::iterator left = pairs.begin();
    std::vector<Pair>::iterator right = pairs.end();

    while (left < right)
    {
        size_t size = right - left;
        std::vector<Pair>::iterator it = left + size / 2;
        if (it->value < toInsert->value)
            left = it + 1;
        else
            right = it;
    }
    return left;
}

void readValues(std::vector<int> &src, std::vector<Pair> &pairs)
{
    for (std::vector<int>::iterator it = src.begin(); it != src.end(); it++)
        pairs.push_back(Pair(*it));
}

void writeResult(std::vector<Pair> &sorted, std::vector<int> &result)
{
    for (std::vector<Pair>::iterator it = sorted.begin(); it != sorted.end(); it++)
        result.push_back(it->value);
}

static void sortTree(std::vector<Pair> &sorted, std::vector<int> &result)
{
    std::vector<Pair> biggers;
    std::vector<Pair> lowers;

    if (!sorted[0].b)
        return writeResult(sorted, result);
    if (sorted[0].a)
        biggers.push_back(*(sorted[0].a));
    biggers.push_back(*sorted[0].b);
    for (std::vector<Pair>::iterator it = sorted.begin() + 1; it != sorted.end(); it++)
    {
        if (it->a)
            lowers.push_back(*it->a);
        biggers.push_back(*it->b);
    }

    // TODO: Respect special insert order
    for (std::vector<Pair>::iterator it = lowers.begin(); it != lowers.end(); it++)
    {
        std::vector<Pair>::iterator insertionPoint = findInsertionPoint(biggers, it);
        biggers.insert(insertionPoint, *it);
    }
    sortTree(biggers, result);
}

static void buildPairsTree(std::vector<Pair> &pairs, std::vector<int> &result)
{
    std::vector<Pair> parents;
    std::vector<Pair>::iterator last = pairs.size() & 1 ? pairs.end() - 1 : pairs.end();
    for (std::vector<Pair>::iterator it = pairs.begin(); it != last; it += 2)
        parents.push_back(Pair(*it, *(it + 1)));
    if (last != pairs.end())
        parents.push_back(Pair(NULL, &(*last)));
    if (parents.size() > 1)
        return buildPairsTree(parents, result);
    std::cout << parents[0] << std::endl;
    sortTree(parents, result);
}

void mergeInsert(std::vector<int> &src)
{
    std::vector<int> result;
    std::vector<Pair> pairs;
    readValues(src, pairs);
    buildPairsTree(pairs, result);
    std::cout << result << std::endl;
}