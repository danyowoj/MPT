#include "TFrac.h"
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

static long gcd(long a, long b)
{
    a = std::abs(a);
    b = std::abs(b);
    while (b)
    {
        long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

TFrac::TFrac(long numerator, long denominator) : num(numerator), den(denominator)
{
    if (den == 0)
        den = 1;
    normalize();
    reduce();
}

TFrac::TFrac(const std::string &str)
{
    fromString(str);
}

TFrac::TFrac(const TFrac &other) : num(other.num), den(other.den) {}

void TFrac::normalize()
{
    if (den < 0)
    {
        num = -num;
        den = -den;
    }
    if (den == 0)
        den = 1;
}

void TFrac::reduce()
{
    long g = gcd(num, den);
    if (g != 0)
    {
        num /= g;
        den /= g;
    }
    normalize();
}

TANumber *TFrac::add(const TANumber &other) const
{
    const TFrac *f = dynamic_cast<const TFrac *>(&other);
    if (!f)
        return copy();
    long newNum = num * f->den + f->num * den;
    long newDen = den * f->den;
    return new TFrac(newNum, newDen);
}

TANumber *TFrac::sub(const TANumber &other) const
{
    const TFrac *f = dynamic_cast<const TFrac *>(&other);
    if (!f)
        return copy();
    long newNum = num * f->den - f->num * den;
    long newDen = den * f->den;
    return new TFrac(newNum, newDen);
}

TANumber *TFrac::mul(const TANumber &other) const
{
    const TFrac *f = dynamic_cast<const TFrac *>(&other);
    if (!f)
        return copy();
    long newNum = num * f->num;
    long newDen = den * f->den;
    return new TFrac(newNum, newDen);
}

TANumber *TFrac::div(const TANumber &other) const
{
    const TFrac *f = dynamic_cast<const TFrac *>(&other);
    if (!f)
        return copy();
    if (f->isZero())
        return new TFrac(0, 1);
    long newNum = num * f->den;
    long newDen = den * f->num;
    return new TFrac(newNum, newDen);
}

TANumber *TFrac::sqr() const
{
    return new TFrac(num * num, den * den);
}

TANumber *TFrac::rev() const
{
    if (isZero())
        return new TFrac(0, 1);
    return new TFrac(den, num);
}

bool TFrac::equals(const TANumber &other) const
{
    const TFrac *f = dynamic_cast<const TFrac *>(&other);
    if (!f)
        return false;
    return num == f->num && den == f->den;
}

bool TFrac::isZero() const
{
    return num == 0;
}

TANumber *TFrac::copy() const
{
    return new TFrac(*this);
}

std::string TFrac::toString() const
{
    if (den == 1)
        return std::to_string(num);
    return std::to_string(num) + "/" + std::to_string(den);
}

void TFrac::fromString(const std::string &str)
{
    std::string s = str;
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
    size_t slash = s.find('/');
    if (slash == std::string::npos)
    {
        num = std::stol(s);
        den = 1;
    }
    else
    {
        std::string numStr = s.substr(0, slash);
        std::string denStr = s.substr(slash + 1);
        num = std::stol(numStr);
        den = std::stol(denStr);
        if (den == 0)
            den = 1;
    }
    normalize();
    reduce();
}

TANumber *TFrac::zero() const
{
    return new TFrac(0, 1);
}
