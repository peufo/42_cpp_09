#include "PmergeMe.hpp"

bool parserArgs(int ac, char **av, std::vector<int> &values)
{
    int value;
    for (int i = 1; i < ac; i++)
    {
        std::stringstream ss(av[i]);
        ss >> value;
        if (ss.fail() || !ss.eof() || value < 0)
        {
            std::cerr << "Args need to be positive integer" << std::endl;
            return false;
        }
        values.push_back(value);
    }
    return true;
}

int main(int ac, char **av)
{
    std::vector<int> values;
    if (!parserArgs(ac, av, values))
        return 1;
    int *data = values.data();
    (void)data;

    std::vector<Slice> s;
    std::vector<Pair> p;
    for (std::vector<int>::iterator it = values.begin(); it != values.end(); it++)
    {
        s.push_back(Slice(it));
        p.push_back(Pair(*it));
    }
    // mergeInsert(s);
    mergeInsertPairs(p);
}