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

TFrac::TFrac(int64_t num, int64_t den) : m_num(num), m_den(den)
{
    if (den == 0)
        throw std::invalid_argument("TFrac: denominator cannot be zero");
    normalize();
}

TFrac::TFrac(const std::string &str) : m_num(0), m_den(1)
{
    // Парсинг: поддерживаем форматы: целое, числитель/знаменатель, целая|числитель/знаменатель
    std::string s = str;
    size_t pipePos = s.find('|');
    size_t slashPos = s.find('/');

    if (pipePos != std::string::npos)
    {
        // Смешанная дробь
        std::string wholeStr = s.substr(0, pipePos);
        std::string fracStr = s.substr(pipePos + 1);
        size_t fracSlash = fracStr.find('/');
        if (fracSlash == std::string::npos)
            throw std::invalid_argument("TFrac: missing '/' in fractional part");
        int64_t whole = std::strtoll(wholeStr.c_str(), nullptr, 10);
        int64_t num = std::strtoll(fracStr.substr(0, fracSlash).c_str(), nullptr, 10);
        int64_t den = std::strtoll(fracStr.substr(fracSlash + 1).c_str(), nullptr, 10);
        if (den == 0)
            throw std::invalid_argument("TFrac: denominator cannot be zero");
        m_num = whole * den + (whole < 0 ? -num : num); // знак whole определяет знак всего числа
        m_den = den;
        normalize();
    }
    else if (slashPos != std::string::npos)
    {
        // Простая дробь
        std::string numStr = s.substr(0, slashPos);
        std::string denStr = s.substr(slashPos + 1);
        m_num = std::strtoll(numStr.c_str(), nullptr, 10);
        m_den = std::strtoll(denStr.c_str(), nullptr, 10);
        if (m_den == 0)
            throw std::invalid_argument("TFrac: denominator cannot be zero");
        normalize();
    }
    else
    {
        // Целое число
        m_num = std::strtoll(s.c_str(), nullptr, 10);
        m_den = 1;
        normalize();
    }
}

#pragma endregion

// Приведение к нормальному виду
void TFrac::normalize()
{
    // Перенос знака в числитель
    if (m_den < 0)
    {
        m_num = -m_num;
        m_den = -m_den;
    }
    if (m_num == 0)
    {
        m_den = 1;
        return;
    }
    int64_t g = gcd(m_num, m_den);
    m_num /= g;
    m_den /= g;
}

#pragma region Арифметические операции

TFrac TFrac::add(const TFrac &other) const
{
    // a1/b1 + a2/b2 = (a1*b2 + a2*b1) / (b1*b2)
    int64_t newNum = m_num * other.m_den + other.m_num * m_den;
    int64_t newDen = m_den * other.m_den;
    return TFrac(newNum, newDen);
}

TFrac TFrac::subtract(const TFrac &other) const
{
    // a1/b1 - a2/b2 = (a1*b2 - a2*b1) / (b1*b2)
    int64_t newNum = m_num * other.m_den - other.m_num * m_den;
    int64_t newDen = m_den * other.m_den;
    return TFrac(newNum, newDen);
}

TFrac TFrac::multiply(const TFrac &other) const
{
    // (a1/b1) * (a2/b2) = (a1*a2) / (b1*b2)
    int64_t newNum = m_num * other.m_num;
    int64_t newDen = m_den * other.m_den;
    return TFrac(newNum, newDen);
}

TFrac TFrac::divide(const TFrac &other) const
{
    if (other.m_num == 0)
        throw std::invalid_argument("TFrac::divide: division by zero");
    int64_t newNum = m_num * other.m_den;
    int64_t newDen = m_den * other.m_num;
    return TFrac(newNum, newDen);
}

TFrac TFrac::square() const { return multiply(*this); }

TFrac TFrac::reciprocal() const
{
    if (m_num == 0)
        throw std::invalid_argument("TFrac::reciprocal: cannot invert zero");
    return TFrac(m_den, m_num);
}

TFrac TFrac::negate() const { return TFrac(-m_num, m_den); }

#pragma endregion

#pragma region Операции сравнения

bool TFrac::equals(const TFrac &other) const { return m_num == other.m_num && m_den == other.m_den; }

bool TFrac::greaterThan(const TFrac &other) const { return m_num * other.m_den > other.m_num * m_den; }

#pragma endregion

#pragma region Десятичное представление

double TFrac::toDouble() const { return static_cast<double>(m_num) / m_den; }

bool TFrac::isTerminatingDecimal() const
{
    int64_t d = m_den;
    while (d % 2 == 0) // Убираем все множители 2
        d /= 2;
    while (d % 5 == 0) // Убираем все множители 5
        d /= 5;
    return d == 1; // Если осталась 1, то конечная десятичная дробь
}

#pragma endregion

std::string TFrac::toString(DisplayFormat fmt) const
{
    if (fmt == DisplayFormat::Decimal && isTerminatingDecimal())
    {
        double val = toDouble();
        // Определим количество знаков после запятой,
        // умножая на 10^n пока не станет целым
        int64_t num = std::llabs(m_num);
        int64_t den = m_den;
        int precision = 0;
        while (den > 1 && num % den != 0)
        {
            num *= 10;
            precision++;
        }
        // Форматируем с фиксированной точностью
        char buf[64];
        snprintf(buf, sizeof(buf), "%.f", precision, val);
        return buf;
    }
    // По умолчнию: дробь
    return std::to_string(m_num) + "/" + std::to_string(m_den);
}
