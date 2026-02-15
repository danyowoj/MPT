#include <History.h>

History::Record::Record(int p1, int p2, const std::string &n1, const std::string &n2)
    : p1(p1), p2(p2), num1(n1), num2(n2) {}

std::string History::Record::toString() const
{
    std::string result = std::to_string(p1) + " : " + num1 + " -> " + std::to_string(p2) + " : " + num2 + "\n";
    return result;
}

History::History() = default; // вектор пуст по умолчанию

void History::addRecord(int p1, int p2, const std::string &n1, const std::string &n2)
{
    records.emplace_back(p1, p2, n1, n2);
}

History::Record History::getRecord(int index) const
{
    if (index < 0 || index >= static_cast<int>(records.size()))
        return Record(0, 0, "", "");
    return records[index];
}

History::Record History::operator[](int index) const { return getRecord(index); }

void History::clear() { records.clear(); }

int History::count() const { return static_cast<int>(records.size()); }
