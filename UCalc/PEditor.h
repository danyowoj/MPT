#ifndef PEDITOR_H
#define PEDITOR_H

#include "TAEditor.h"

class PEditor : public TAEditor
{
private:
    int base;           // основание 2..16
    bool separatorUsed; // уже введён разделитель
    bool isNegative;    // признак отрицательности (для быстрой проверки)

    bool isValidDigit(char c) const;
    std::string normalize() const; // удалить лишние нули в начале/конце
public:
    PEditor(int b = 10); // основание по умолчанию 10
    PEditor(const PEditor &other);
    ~PEditor() override {}

    std::string addDigit(int digit) override;
    std::string addSign() override;
    std::string addSeparator() override;
    std::string backspace() override;
    std::string clear() override;

    bool isZero() const override;

    std::string getString() const override;
    void setString(const std::string &str) override;

    int getBase() const { return base; }
    void setBase(int b); // смена основания (сброс)

    TANumber *createNumber() const override;
};

#endif
