#pragma once
#include <string>
#include <vector>

class History
{
public:
    struct Record
    {
        int p1;
        int p2;
        std::string num1;
        std::string num2;

        Record(int p1, int p2, const std::string &n1, const std::string &n2);
        std::string toString() const;
    };

    History();

    // Добавить запись в историю
    void addRecord(int p1, int p2, const std::string &n1, const std::string &n2);

    // Получить запись по индексу
    Record getRecord(int index) const;

    // Оператор доступа по индексу
    Record operator[](int index) const;

    // Очистить историю
    void clear();

    // Количество записей в итсории
    int count() const;

    std::vector<Record> records;
};
