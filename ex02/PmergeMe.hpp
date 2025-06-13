#ifndef PMERGEME_HPP
#define PMERGEME_HPP

class PmergeMe
{
private:
protected:
public:
    ~PmergeMe();
    PmergeMe();
    PmergeMe(const PmergeMe &src);
    PmergeMe &operator=(const PmergeMe &src);
};

PmergeMe::~PmergeMe()
{
}

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe &src)
{
}

PmergeMe &PmergeMe::operator=(const PmergeMe &src)
{
    if (this == &src)
        return *this;

    return *this;
}

#endif
