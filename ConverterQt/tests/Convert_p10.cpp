#include "Convert_p10.h"

int Convert_p10::charToNum(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';

    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;

    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;

    // Некорректный символ
    return -1;
}

double Convert_p10::convert(const std::string &digits, int p, double weight)
{
    double result = 0.0;

    for (char ch : digits)
    {
        int value = charToNum(ch);
        if (value < 0 || value >= p)
            return 0.0; // защита от мусора

        result += value * weight;
        weight /= p;
    }

    return result;
}

double Convert_p10::Do(const std::string &p_num, int p)
{
    if (p < 2 || p > 16 || p_num.empty())
        return 0.0;

    std::string s = p_num;
    bool negative = false;

    // 1. Знак
    if (s[0] == '-')
    {
        negative = true;
        s.erase(0, 1);
    }

    // 2. Найти точку
    size_t dotPos = s.find('.');
    std::string intPart = s;
    std::string fracPart;

    if (dotPos != std::string::npos)
    {
        intPart = s.substr(0, dotPos);
        fracPart = s.substr(dotPos + 1);
    }

    // 3. Вес старшего разряда целой части
    double intWeight = 1.0;
    for (size_t i = 1; i < intPart.size(); ++i)
        intWeight *= p;

    double result = 0.0;

    // 4. Целая часть
    if (!intPart.empty())
        result += convert(intPart, p, intWeight);

    // 5. Дробная часть
    if (!fracPart.empty())
        result += convert(fracPart, p, 1.0 / p);

    return negative ? -result : result;
}
