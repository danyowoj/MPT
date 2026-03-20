#ifndef TCOMPLEX_H
#define TCOMPLEX_H

#include "TANumber.h"

class TComplex : public TANumber
{
private:
    double re;
    double im;

public:
    TComplex(double real = 0.0, double imag = 0.0);
    TComplex(const std::string &str);
    TComplex(const TComplex &other);
    ~TComplex() override {}

    TANumber *add(const TANumber &other) const override;
    TANumber *sub(const TANumber &other) const override;
    TANumber *mul(const TANumber &other) const override;
    TANumber *div(const TANumber &other) const override;

    TANumber *sqr() const override;
    TANumber *rev() const override;

    bool equals(const TANumber &other) const override;
    bool isZero() const override;

    TANumber *copy() const override;

    std::string toString() const override;
    void fromString(const std::string &str) override;
};

#endif
