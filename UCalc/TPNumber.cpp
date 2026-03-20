#include "TPNumber.h"
#include <cmath>
#include <sstream>
#include <cctype>
#include <algorithm>

const std::string TPNumber::digits = "0123456789ABCDEF";

TPNumber::TPNumber(double val, int base) : value(val), base(base)
{
    if (base < 2 || base > 16)
        base = 10;
}

TPNumber::TPNumber(const std::string &str, int base) : base(base)
{
    if (base < 2 || base > 16)
        base = 10;
    value = parseString(str);
}

TPNumber::TPNumber(const TPNumber &other) : value(other.value), base(other.base) {}

double TPNumber::parseString(const std::string &str) const
{
    std::string s = str;
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
    bool negative = false;
    if (!s.empty() && s[0] == '-')
    {
        negative = true;
        s = s.substr(1);
    }
    size_t dotPos = s.find('.');
    if (dotPos == std::string::npos)
        dotPos = s.find(',');
    std::string intPart = (dotPos == std::string::npos) ? s : s.substr(0, dotPos);
    std::string fracPart = (dotPos == std::string::npos) ? "" : s.substr(dotPos + 1);

    double result = 0.0;
    for (char ch : intPart)
    {
        ch = toupper(ch);
        size_t pos = digits.find(ch);
        if (pos == std::string::npos || pos >= (size_t)base)
            pos = 0;
        result = result * base + pos;
    }
    double factor = 1.0 / base;
    for (char ch : fracPart)
    {
        ch = toupper(ch);
        size_t pos = digits.find(ch);
        if (pos == std::string::npos || pos >= (size_t)base)
            pos = 0;
        result += pos * factor;
        factor /= base;
    }
    return negative ? -result : result;
}

std::string TPNumber::formatString(double val) const
{
    if (std::isnan(val) || std::isinf(val))
        return "NaN";
    bool negative = false;
    if (val < 0)
    {
        negative = true;
        val = -val;
    }
    long long intPart = (long long)val;
    double fracPart = val - intPart;

    std::string intStr;
    if (intPart == 0)
        intStr = "0";
    else
    {
        while (intPart > 0)
        {
            int digit = intPart % base;
            intPart /= base;
            intStr = digits[digit] + intStr;
        }
    }

    std::string fracStr;
    const int maxFracDigits = 10;
    double f = fracPart;
    for (int i = 0; i < maxFracDigits && f > 1e-10; ++i)
    {
        f *= base;
        int digit = (int)f;
        fracStr += digits[digit];
        f -= digit;
    }
    while (!fracStr.empty() && fracStr.back() == '0')
        fracStr.pop_back();

    std::string result;
    if (negative)
        result += '-';
    result += intStr;
    if (!fracStr.empty())
    {
        result += '.';
        result += fracStr;
    }
    return result;
}

TANumber *TPNumber::add(const TANumber &other) const
{
    const TPNumber *p = dynamic_cast<const TPNumber *>(&other);
    if (!p)
        return copy();
    return new TPNumber(value + p->value, base);
}

TANumber *TPNumber::sub(const TANumber &other) const
{
    const TPNumber *p = dynamic_cast<const TPNumber *>(&other);
    if (!p)
        return copy();
    return new TPNumber(value - p->value, base);
}

TANumber *TPNumber::mul(const TANumber &other) const
{
    const TPNumber *p = dynamic_cast<const TPNumber *>(&other);
    if (!p)
        return copy();
    return new TPNumber(value * p->value, base);
}

TANumber *TPNumber::div(const TANumber &other) const
{
    const TPNumber *p = dynamic_cast<const TPNumber *>(&other);
    if (!p)
        return copy();
    if (p->isZero())
        return new TPNumber(0.0, base);
    return new TPNumber(value / p->value, base);
}

TANumber *TPNumber::sqr() const
{
    return new TPNumber(value * value, base);
}

TANumber *TPNumber::rev() const
{
    if (isZero())
        return new TPNumber(0.0, base);
    return new TPNumber(1.0 / value, base);
}

bool TPNumber::equals(const TANumber &other) const
{
    const TPNumber *p = dynamic_cast<const TPNumber *>(&other);
    if (!p)
        return false;
    const double eps = 1e-12;
    return std::abs(value - p->value) < eps && base == p->base;
}

bool TPNumber::isZero() const
{
    const double eps = 1e-12;
    return std::abs(value) < eps;
}

TANumber *TPNumber::copy() const
{
    return new TPNumber(*this);
}

std::string TPNumber::toString() const
{
    return formatString(value);
}

void TPNumber::fromString(const std::string &str)
{
    value = parseString(str);
}
