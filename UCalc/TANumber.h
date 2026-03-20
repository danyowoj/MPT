#ifndef TANUMBER_H
#define TANUMBER_H

#include <string>

class TANumber
{
public:
    virtual ~TANumber() {}

    // Арифметические операции (возвращают новый объект в куче)
    virtual TANumber *add(const TANumber &other) const = 0;
    virtual TANumber *sub(const TANumber &other) const = 0;
    virtual TANumber *mul(const TANumber &other) const = 0;
    virtual TANumber *div(const TANumber &other) const = 0;

    // Унарные операции
    virtual TANumber *sqr() const = 0;
    virtual TANumber *rev() const = 0; // обратное число

    // Сравнение
    virtual bool equals(const TANumber &other) const = 0;
    virtual bool isZero() const = 0;

    // Клонирование
    virtual TANumber *copy() const = 0;

    // Преобразование в строку и из строки
    virtual std::string toString() const = 0;
    virtual void fromString(const std::string &str) = 0;
};

#endif
