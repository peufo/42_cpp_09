#include "RPN.hpp"

void test(const std::string &str, double expected)
{
    RPN rpn;
    try
    {
        double result = rpn.compute(str);
        std::cout << "[ " << str << " ] = " << result << '\n';
        if (result == expected)
            std::cout << "SUCCESS\n"
                      << std::endl;
        else
            std::cout << "ERROR\n"
                      << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error with `" << str << "`\n"
                  << e.what() << "\n\n";
    }
}

int main(int ac, char **av)
{
    RPN rpn;

    if (ac > 1)
    {
        double result = rpn.compute(av[1]);
        std::cout << result << std::endl;
        return 0;
    }

    test("3 4 + 5 6 + *", 77);
    test("8 9 * 9 - 9 - 9 - 4 - 1 +", 42);
    test("7 7 * 7 -", 42);
    test("1 2 * 2 / 2 * 2 4 - +", 0);
    test("(1 + 1)", 0);
    test("1 1", 0);
    test("+ +", 0);
    test("1 +", 0);
    test("1 1 1 + -", -1);
    test("1 1 + 1 -", 1);
    return 0;
}