#include "TComplex.h"
#include <sstream>
#include <cmath>
#include <algorithm>
#include <cctype>

static const double EPS = 1e-12;

TComplex::TComplex(double real, double imag) : re(real), im(imag) {}

TComplex::TComplex(const std::string &str)
{
    fromString(str);
}

TComplex::TComplex(const TComplex &other) : re(other.re), im(other.im) {}

TANumber *TComplex::add(const TANumber &other) const
{
    const TComplex *c = dynamic_cast<const TComplex *>(&other);
    if (!c)
        return copy();
    return new TComplex(re + c->re, im + c->im);
}

TANumber *TComplex::sub(const TANumber &other) const
{
    const TComplex *c = dynamic_cast<const TComplex *>(&other);
    if (!c)
        return copy();
    return new TComplex(re - c->re, im - c->im);
}

TANumber *TComplex::mul(const TANumber &other) const
{
    const TComplex *c = dynamic_cast<const TComplex *>(&other);
    if (!c)
        return copy();
    double real = re * c->re - im * c->im;
    double imag = re * c->im + im * c->re;
    return new TComplex(real, imag);
}

TANumber *TComplex::div(const TANumber &other) const
{
    const TComplex *c = dynamic_cast<const TComplex *>(&other);
    if (!c)
        return copy();
    double denom = c->re * c->re + c->im * c->im;
    if (std::abs(denom) < EPS)
        return new TComplex(0, 0);
    double real = (re * c->re + im * c->im) / denom;
    double imag = (im * c->re - re * c->im) / denom;
    return new TComplex(real, imag);
}

TANumber *TComplex::sqr() const
{
    return new TComplex(re * re - im * im, 2 * re * im);
}

TANumber *TComplex::rev() const
{
    double denom = re * re + im * im;
    if (std::abs(denom) < EPS)
        return new TComplex(0, 0);
    return new TComplex(re / denom, -im / denom);
}

bool TComplex::equals(const TANumber &other) const
{
    const TComplex *c = dynamic_cast<const TComplex *>(&other);
    if (!c)
        return false;
    return std::abs(re - c->re) < EPS && std::abs(im - c->im) < EPS;
}

bool TComplex::isZero() const
{
    return std::abs(re) < EPS && std::abs(im) < EPS;
}

TANumber *TComplex::copy() const
{
    return new TComplex(*this);
}

std::string TComplex::toString() const
{
    std::ostringstream oss;
    oss << re;
    if (im >= 0)
        oss << " + i*";
    else
        oss << " - i*";
    oss << std::abs(im);
    return oss.str();
}

void TComplex::fromString(const std::string &str)
{
    std::string s = str;
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
    size_t pos = s.find("i*");
    if (pos == std::string::npos)
    {
        re = std::stod(s);
        im = 0;
        return;
    }
    std::string rePart = s.substr(0, pos);
    std::string imPart = s.substr(pos + 2);
    char sign = '+';
    if (!rePart.empty())
    {
        char last = rePart.back();
        if (last == '+' || last == '-')
        {
            sign = last;
            rePart.pop_back();
        }
    }
    re = std::stod(rePart);
    im = std::stod(imPart);
    if (sign == '-')
        im = -im;
}
