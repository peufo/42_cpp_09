#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
    : filename("data.csv"),
      separator(','),
      maxAmount(std::numeric_limits<int>::max())
{
    this->loadFile();
}

BitcoinExchange::BitcoinExchange(const char *filename, char separator, int maxAmount)
    : filename(filename),
      separator(separator),
      maxAmount(maxAmount)
{
    this->loadFile();
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &src)
    : std::list<entrie_t>(src),
      filename(src.filename),
      separator(src.separator),
      maxAmount(src.maxAmount)
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &src)
{
    if (this == &src)
        return *this;
    this->filename = src.filename;
    this->separator = src.separator;
    this->maxAmount = src.maxAmount;
    this->clear();
    this->loadFile();
    return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

const entrie_t &BitcoinExchange::findEntrieBefore(const entrie_t &entrie) const
{
    for (BitcoinExchange::const_iterator i = this->end(); i != this->begin(); i--)
    {
        if (i->time <= entrie.time)
            return (*i);
    }
    return this->front();
}

void BitcoinExchange::parseLine(const std::string &line)
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
    this->push_back((entrie_t){.date = date,
                               .time = std::mktime(&date),
                               .amount = amount});
}

void BitcoinExchange::loadFile()
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
                this->parseLine(line);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error " << this->filename << ":" << lineNo << "\t" << e.what() << '\n';
        }
        lineNo++;
    }
}
