#include "convert_10p.h"
#include <algorithm>
#include <cmath>

char convert_10p::intToChar(int n)
{
    if (n >= 0 && n <= 9)
        return static_cast<char>('0' + n);

    if (n >= 10 && n <= 15)
        return static_cast<char>('A' + (n - 10));

    return '?'; // защита
}

std::string convert_10p::intToP(long long value, int p)
{
    if (value == 0)
        return "0";

    std::string result;

    while (value > 0)
    {
        int digit = static_cast<int>(value % p);
        result.push_back(intToChar(digit));
        value /= p;
    }

    std::reverse(result.begin(), result.end());
    return result;
}

std::string convert_10p::fltToP(double frac, int p, int precision)
{
    std::string result;

    for (int i = 0; i < precision; ++i)
    {
        frac *= p;
        int digit = static_cast<int>(frac);
        result.push_back(intToChar(digit));
        frac -= digit;
    }

    return result;
}

std::string convert_10p::Do(double value, int p, int precision)
{
    if (p < 2 || p > 16 || precision < 0)
        return "0";

    bool negative = (value < 0);
    value = std::fabs(value);

    long long intPart = static_cast<long long>(value);
    double fracPart = value - intPart;

    std::string result;

    if (negative)
        result.push_back('-');

    // целая часть
    result += intToP(intPart, p);

    // дробная часть
    if (precision > 0)
    {
        result.push_back('.');
        result += fltToP(fracPart, p, precision);
    }

    return result;
}