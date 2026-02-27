#include "../include/UFrac.h"
#include <cstdlib>
#include <cctype>
#include <algorithm>
#include <cmath>

// НОД
int64_t gcd(int64_t a, int64_t b)
{
    a = std::llabs(a);
    b = std::llabs(b);
    while (b != 0)
    {
        int64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

#pragma region Конструкторы

TFrac::TFrac() : numerator(0), denominator(1) {}

TFrac::TFrac(int64_t num, int64_t den) : numerator(num), denominator(den)
{
    if (den == 0)
        throw std::invalid_argument("TFrac: denominator cannot be zero");
    normalize();
}

TFrac::TFrac(const std::string &str) : numerator(0), denominator(1)
{
    std::string s = str;

    size_t pipePos = s.find('|');
    size_t slashPos = s.find('/');

    if (pipePos != std::string::npos)
    {
        // Формат: целая часть | числитель/знаменатель
        std::string wholePartStr = s.substr(0, pipePos);
        std::string fracPartStr = s.substr(pipePos + 1);

        // Парсим целую часть (может быть со знаком)
        char *end;
        int64_t whole = std::strtoll(wholePartStr.c_str(), &end, 10);
        if (*end != '\0')
            throw std::invalid_argument("TFrac: invalid whole part in mixed fraction");

        // Парсим дробную часть (должна содержать '/')
        size_t fracSlah = fracPartStr.find('/');
        if (fracSlah == std::string::npos)
            throw std::invalid_argument("TFrac: missing '/' in fractional part of mixed fraction");

        std::string numStr = fracPartStr.substr(0, fracSlah);
        std::string denStr = fracPartStr.substr(fracSlah + 1);

        int64_t num = std::strtoll(numStr.c_str(), &end, 10);
        if (*end != '\0')
            throw std::invalid_argument("TFrac: invalid numerator in mixed fraction");

        int64_t den = std::strtoll(denStr.c_str(), &end, 10);
        if (*end != '\0')
            throw std::invalid_argument("TFrac: invalid denominator in mixed fraction");

        if (den == 0)
            throw std::invalid_argument("TFrac: denominator cannot be zero in mixed fraction");

        // Вычисляем: (whole * den + num) / den
        numerator = whole * den + num;
        denominator = den;
        normalize();
    }
    else if (slashPos != std::string::npos)
    {
        // Простая дробь вида числитель/знаменатель
        std::string numStr = s.substr(0, slashPos);
        std::string denStr = s.substr(slashPos + 1);

        char *end;
        int64_t num = std::strtoll(numStr.c_str(), &end, 10);
        if (*end != '\0')
            throw std::invalid_argument("TFrac: invalid numerator");

        int64_t den = std::strtoll(denStr.c_str(), &end, 10);
        if (*end != '\0')
            throw std::invalid_argument("TFrac: invalid denominator");

        if (den == 0)
            throw std::invalid_argument("TFrac: denominator cannot be zero");

        numerator = num;
        denominator = den;
        normalize();
    }
    else
    {
        // Нет разделителей — целое число
        char *end;
        int64_t num = std::strtoll(s.c_str(), &end, 10);
        if (*end != '\0')
            throw std::invalid_argument("TFrac: invalid integer format");
        numerator = num;
        denominator = 1;
        normalize();
    }
}

#pragma endregion

// Приведение к нормальному виду
void TFrac::normalize()
{
    // Знак переносим в числитель
    if (denominator < 0)
    {
        numerator = -numerator;
        denominator = -denominator;
    }

    // Сокращение на НОД
    if (numerator == 0)
    {
        denominator = 1;
    }
    else
    {
        int64_t g = gcd(std::llabs(numerator), std::llabs(denominator));
        numerator /= g;
        denominator /= g;
    }
}

#pragma region Арифметические операции

TFrac TFrac::add(const TFrac &other) const
{
    // a1/b1 + a2/b2 = (a1*b2 + a2*b1) / (b1*b2)
    int64_t newNum = numerator * other.denominator + other.numerator * denominator;
    int64_t newDen = denominator * other.denominator;
    return TFrac(newNum, newDen);
}

TFrac TFrac::subtract(const TFrac &other) const
{
    // a1/b1 - a2/b2 = (a1*b2 - a2*b1) / (b1*b2)
    int64_t newNum = numerator * other.denominator - other.numerator * denominator;
    int64_t newDen = denominator * other.denominator;
    return TFrac(newNum, newDen);
}

TFrac TFrac::multiply(const TFrac &other) const
{
    // (a1/b1) * (a2/b2) = (a1*a2) / (b1*b2)
    int64_t newNum = numerator * other.numerator;
    int64_t newDen = denominator * other.denominator;
    return TFrac(newNum, newDen);
}

TFrac TFrac::divide(const TFrac &other) const
{
    if (other.numerator == 0)
        throw std::invalid_argument("TFrac::divide: division by zero");
    // (a1/b1) / (a2/b2) = (a1*b2) / (a2*b1)
    int64_t newNum = numerator * other.denominator;
    int64_t newDen = denominator * other.numerator;
    return TFrac(newNum, newDen);
}

TFrac TFrac::square() const
{
    // (a/b)^2 = (a*a)/(b*b)
    return multiply(*this);
}

TFrac TFrac::reciprocal() const
{
    if (numerator == 0)
        throw std::invalid_argument("TFrac::reciprocal: cannot invert zero");
    return TFrac(denominator, numerator);
}

TFrac TFrac::negate() const
{
    return TFrac(-numerator, denominator);
}

#pragma endregion

#pragma region Операции сравнения

bool TFrac::equals(const TFrac &other) const { return (numerator == other.numerator) && (denominator == other.denominator); }

bool TFrac::greaterThan(const TFrac &other) const { return numerator * other.denominator > other.numerator * denominator; }

#pragma endregion

#pragma region Геттеры

int64_t TFrac::getNumerator() const { return numerator; }

int64_t TFrac::getDenominator() const { return denominator; }

std::string TFrac::getNumeratorString() const { return std::to_string(numerator); }

std::string TFrac::getDenominatorString() const { return std::to_string(denominator); }

std::string TFrac::toString() const { return std::to_string(numerator) + "/" + std::to_string(denominator); }

#pragma endregion

double TFrac::toDouble() const { return static_cast<double>(numerator) / static_cast<double>(denominator); }

bool TFrac::isTerminatingDecimal() const
{
    int64_t d = denominator;
    // Убираем все множители 2
    while (d % 2 == 0)
        d /= 2;
    // Убираем все множители 5
    while (d % 5 == 0)
        d /= 5;
    // Если осталась 1, то конечная десятичная дробь
    return d == 1;
}
