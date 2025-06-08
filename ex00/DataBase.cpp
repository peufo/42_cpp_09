#include "DataBase.hpp"

DataBase::DataBase(const char *filename, char separator, int maxAmount) : filename(filename), separator(separator), maxAmount(maxAmount)
{
    this->loadFile();
}

entrie_t DataBase::parseLine(const std::string &line)
{
    std::istringstream is(line);
    char sepDate1, sepDate2;
    int year, month, day;
    float amount;
    is >> year >> sepDate1 >> month >> sepDate2 >> day;
    if (is.fail())
        throw std::invalid_argument("Parsing date failed");
    if (sepDate1 != '-' || sepDate2 != '-')
        throw std::invalid_argument("Incorrect date separator");
    if (month < 1 || month > 12)
        throw std::invalid_argument("Month is out of range 01-12");
    if (day < 1 || day > 31)
        throw std::invalid_argument("Day is out of range 01-31");
    while (is.peek() == ' ')
    {
        is.get();
        if (is.eof())
            throw std::invalid_argument(std::string("Fields separator ` ") + this->separator + " ` not found");
    }
    if (is.get() != this->separator)
        throw std::invalid_argument("Field separator invalid");
    if (is.eof())
        throw std::invalid_argument("Amount not found");
    is >> amount;
    if (is.fail())
        throw std::invalid_argument("Parsing amount failed");
    if (amount < 0)
        throw std::invalid_argument("Not a positive amount");
    if (amount > this->maxAmount)
        throw std::invalid_argument("Too large amount");
    std::tm date = {};
    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;
    return (entrie_t){
        .date = date,
        .time = std::mktime(&date),
        .amount = amount};
}

void DataBase::loadFile()
{
    std::ifstream file(this->filename);
    if (!file.good())
        throw std::runtime_error(std::string("Cannot open file `") + this->filename + "`");
    std::string line;
    getline(file, line);
    int lineNo = 2;
    while (!file.eof())
    {
        getline(file, line);
        try
        {
            if (line.length())
                this->push_back(parseLine(line));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error " << this->filename << ":" << lineNo << "\t" << e.what() << '\n';
        }
        lineNo++;
    }
}
