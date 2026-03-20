#ifndef TPNUMBER_H
#define TPNUMBER_H

#include "TANumber.h"

class TPNumber : public TANumber
{
private:
    double value; // внутреннее представление числа
    int base;     // основание 2..16

    static const std::string digits;

    double parseString(const std::string &str) const;
    std::string formatString(double val) const;

public:
    TPNumber(double val = 0.0, int base = 10);
    TPNumber(const std::string &str, int base);
    TPNumber(const TPNumber &other);
    ~TPNumber() override {}

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

    int getBase() const { return base; }
    void setBase(int newBase)
    {
        if (newBase >= 2 && newBase <= 16)
            base = newBase;
    }
};

#endif
