#ifndef TFRAC_H
#define TFRAC_H

#include "TANumber.h"

class TFrac : public TANumber
{
private:
    long num; // числитель
    long den; // знаменатель (>0)
    void reduce();
    void normalize(); // перенос знака в числитель

public:
    TFrac(long numerator = 0, long denominator = 1);
    TFrac(const std::string &str);
    TFrac(const TFrac &other);
    ~TFrac() override {}

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

    long getNum() const { return num; }
    long getDen() const { return den; }
};

#endif