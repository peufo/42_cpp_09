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
    if (!values.size())
    {
        std::cout << "Nothing to sort !" << std::endl;
        return 0;
    }
    mergeInsert(values);

    return 1;
}