#include "../include/UFrac.h"
#include <cstdlib>
#include <cctype>
#include <algorithm>
#include <cmath>

// НОД
static int64_t gcd(int64_t a, int64_t b)
{
    a = std::llabs(a);
    b = std::llabs(b);
    while (b)
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
    std::string s = str;
    size_t pipePos = s.find('|');
    size_t slashPos = s.find('/');
    char *end;

    if (pipePos != std::string::npos)
    {
        // Смешанная дробь вида: целая|числитель/знаменатель
        std::string wholeStr = s.substr(0, pipePos);
        std::string fracStr = s.substr(pipePos + 1);
        size_t fracSlash = fracStr.find('/');
        if (fracSlash == std::string::npos)
            throw std::invalid_argument("TFrac: missing '/' in fractional part");

        // Парсим целую часть
        int64_t whole = std::strtoll(wholeStr.c_str(), &end, 10);
        if (*end != '\0')
            throw std::invalid_argument("TFrac: invalid whole part");

        // Парсим числитель
        std::string numStr = fracStr.substr(0, fracSlash);
        int64_t num = std::strtoll(numStr.c_str(), &end, 10);
        if (*end != '\0')
            throw std::invalid_argument("TFrac: invalid numerator");

        // Парсим знаменатель
        std::string denStr = fracStr.substr(fracSlash + 1);
        int64_t den = std::strtoll(denStr.c_str(), &end, 10);
        if (*end != '\0')
            throw std::invalid_argument("TFrac: invalid denominator");
        if (den == 0)
            throw std::invalid_argument("TFrac: denominator cannot be zero");

        // Вычисляем значение: целая часть * знаменатель + числитель (с учётом их знаков)
        m_num = whole * den + num;
        m_den = den;
        normalize();
    }
    else if (slashPos != std::string::npos)
    {
        // Простая дробь вида: числитель/знаменатель
        std::string numStr = s.substr(0, slashPos);
        std::string denStr = s.substr(slashPos + 1);

        int64_t num = std::strtoll(numStr.c_str(), &end, 10);
        if (*end != '\0')
            throw std::invalid_argument("TFrac: invalid numerator");

        int64_t den = std::strtoll(denStr.c_str(), &end, 10);
        if (*end != '\0')
            throw std::invalid_argument("TFrac: invalid denominator");
        if (den == 0)
            throw std::invalid_argument("TFrac: denominator cannot be zero");

        m_num = num;
        m_den = den;
        normalize();
    }
    else
    {
        // Целое число
        int64_t num = std::strtoll(s.c_str(), &end, 10);
        if (*end != '\0')
            throw std::invalid_argument("TFrac: invalid integer format");
        m_num = num;
        m_den = 1;
        // нормализация не нужна, но для единообразия вызовем
        normalize();
    }
}

#pragma endregion

// Приведение к нормальному виду
void TFrac::normalize()
{
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

TFrac TFrac::add(const TFrac &other) const { return TFrac(m_num * other.m_den + other.m_num * m_den, m_den * other.m_den); }

TFrac TFrac::subtract(const TFrac &other) const { return TFrac(m_num * other.m_den - other.m_num * m_den, m_den * other.m_den); }

TFrac TFrac::multiply(const TFrac &other) const { return TFrac(m_num * other.m_num, m_den * other.m_den); }

TFrac TFrac::divide(const TFrac &other) const
{
    if (other.m_num == 0)
        throw std::invalid_argument("TFrac::divide: division by zero");
    return TFrac(m_num * other.m_den, m_den * other.m_num);
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
    while (d % 2 == 0)
        d /= 2;
    while (d % 5 == 0)
        d /= 5;
    return d == 1;
}

#pragma endregion

std::string TFrac::toString(DisplayFormat fmt) const
{
    if (fmt == DisplayFormat::Decimal && isTerminatingDecimal())
    {
        // Определяем необходимое количество знаков после запятой
        int64_t den = m_den;
        int pow2 = 0, pow5 = 0;
        while (den % 2 == 0)
        {
            den /= 2;
            ++pow2;
        }
        while (den % 5 == 0)
        {
            den /= 5;
            ++pow5;
        }
        int precision = std::max(pow2, pow5);

        double val = toDouble();
        char buf[64];
        snprintf(buf, sizeof(buf), "%.*f", precision, val);
        return buf;
    }
    // По умолчанию формат дроби
    return std::to_string(m_num) + "/" + std::to_string(m_den);
}
