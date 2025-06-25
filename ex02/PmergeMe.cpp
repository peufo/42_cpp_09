#include "PmergeMe.hpp"

DEBUG(int compPairing = 0;)
DEBUG(int compInsertion = 0;)

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
    return os << pairs;
}

std::ostream &operator<<(std::ostream &os, std::vector<int> &values)
{
    for (std::vector<int>::iterator it = values.begin(); it != values.end(); it++)
        os << *it << ' ';
    return os;
}

std::ostream &operator<<(std::ostream &os, std::vector<Pair> &pairs)
{
    for (std::vector<Pair>::iterator it = pairs.begin(); it != pairs.end(); it++)
        os << it->value << ' ';
    return os;
}

Pair *getPairNoNull(std::vector<Pair *> &pairs)
{
    for (std::vector<Pair *>::iterator it = pairs.begin(); it != pairs.end(); it++)
        if (*it)
            return *it;
    return NULL;
}

std::ostream &operator<<(std::ostream &os, std::vector<Pair *> &pairs)
{
    std::vector<Pair *> children;
    Pair *pair = getPairNoNull(pairs);
    if (!pair)
        return os;
    int deep = pair->getDeep();
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
        {
            os << std::setw(padding + 1) << 'x';
            children.push_back(NULL);
            children.push_back(NULL);
        }
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
    DEBUG(compPairing++;)
    if (this->a->value > this->b->value)
        this->swap();
}

Pair::Pair(Pair *a, Pair *b) : value(b->value), a(a), b(b)
{
    if (!this->a || !this->b)
        return;
    DEBUG(compPairing++;)
    if (this->a->value > this->b->value)
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

//*************************************************************************
//*******************              UTILS              *********************
//*************************************************************************

void setSpecialOrder(int len, std::vector<size_t> &order)
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

static timeval getTime()
{
    timeval now;
    gettimeofday(&now, NULL);
    return now;
}

static double msElapsedFrom(timeval start)
{
    timeval now;

    gettimeofday(&now, NULL);
    long seconds = now.tv_sec - start.tv_sec;
    long microseconds = now.tv_usec - start.tv_usec;
    return (seconds * 1000) + (microseconds / 1000.0);
}

//*************************************************************************
//*******************       VECTOR IMPLEMENTAION      *********************
//*************************************************************************

static void readValues(std::vector<int> &src, std::vector<Pair> &pairs)
{
    for (std::vector<int>::iterator it = src.begin(); it != src.end(); it++)
        pairs.push_back(Pair(*it));
}

static void writeResult(std::vector<Pair> &sorted, std::vector<int> &result)
{
    for (std::vector<Pair>::iterator it = sorted.begin(); it != sorted.end(); it++)
        result.push_back(it->value);
}

static std::vector<Pair>::iterator findInsertionPoint(std::vector<Pair> &pairs, Pair &toInsert, size_t limitRight)
{
    std::vector<Pair>::iterator left = pairs.begin();
    std::vector<Pair>::iterator right = pairs.begin() + limitRight + 1;
    while (left < right)
    {
        size_t size = right - left;
        std::vector<Pair>::iterator it = left + size / 2;
        DEBUG(compInsertion++;)
        if (it->value <= toInsert.value)
            left = it + 1;
        else
            right = it;
    }
    return left;
}

static void insertInBiggers(Pair &pair, std::vector<Pair> &biggers, size_t limitRight)
{
    std::vector<Pair>::iterator insertionPoint = findInsertionPoint(biggers, pair, limitRight);
    DEBUG(std::cout << std::setw(5) << pair.value << " -> " << biggers << std::endl;)
    biggers.insert(insertionPoint, pair);
}

static void sortTree(std::vector<Pair> &sorted, std::vector<int> &result)
{
    std::vector<Pair> biggers;
    std::vector<Pair> lowers;
    std::vector<size_t> limitsRight;

    if (!sorted[0].b)
        return writeResult(sorted, result);
    if (sorted[0].a)
        biggers.push_back(*(sorted[0].a));
    size_t biggerIndex = biggers.size();

    for (std::vector<Pair>::iterator it = sorted.begin(); it != sorted.end(); it++)
        biggers.push_back(*it->b);

    for (std::vector<Pair>::iterator it = sorted.begin() + 1; it != sorted.end(); it++)
    {
        if (it->a)
        {
            lowers.push_back(*it->a);
            limitsRight.push_back(biggerIndex);
        }
        biggerIndex++;
    }

    DEBUG(std::cout << "\nPending: " << lowers << std::endl;)

    size_t nbInserted = 0;
    std::vector<size_t> order;
    setSpecialOrder(lowers.size(), order);
    for (size_t i = 0; i < lowers.size(); i++)
        insertInBiggers(lowers[order[i]], biggers, limitsRight[order[i]] + nbInserted++);

    // for (size_t i = 0; i < lowers.size(); i++)
    //     insertInBiggers(lowers[i], biggers, limitsRight[i] + nbInserted++);

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
    DEBUG(std::cout << parents[0] << std::endl;)
    sortTree(parents, result);
}

void mergeInsert(std::vector<int> &src)
{
    DEBUG(compPairing = 0;)
    DEBUG(compInsertion = 0;)

    std::vector<int> result;
    std::vector<Pair> pairs;
    timeval start = getTime();
    readValues(src, pairs);
    buildPairsTree(pairs, result);
    double elapsed = msElapsedFrom(start);

    DEBUG(std::cout << "\nComparison: "
                    << compPairing << " pairing + "
                    << compInsertion << " insertion = "
                    << compPairing + compInsertion << std::endl;)

    std::cout << "Before: " << src << std::endl;
    std::cout << "After:  " << result << std::endl;
    std::cout << "Time to process " << src.size() << " elements with std::vector : " << elapsed << " ms" << std::endl;
}