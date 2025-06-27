#include "PmergeMe.hpp"

bool parserArgs(int ac, char **av, std::vector<int> &vector, std::deque<int> &deque)
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
        vector.push_back(value);
        deque.push_back(value);
    }
    return true;
}

int main(int ac, char **av)
{
    std::vector<int> vector;
    std::deque<int> deque;
    if (!parserArgs(ac, av, vector, deque))
        return 1;
    if (!vector.size())
    {
        std::cout << "Nothing to sort !" << std::endl;
        return 0;
    }
    mergeInsert(vector);
    mergeInsert(deque);
    return 0;
}