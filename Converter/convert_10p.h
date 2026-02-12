#pragma once

#include <string>

class convert_10p
{
public:
    // Перевод десятичного числа в систему счисления p
    // @param value Десятичное число
    // @param p Основание системы счисления
    // @param precision Точность
    static std::string Do(double value, int p, int precision);

private:
    // Преобразовать число в символ
    static char intToChar(int n);
    // Целая часть
    static std::string intToP(long long value, int p);
    // Дробная часть
    static std::string fltToP(double frac, int p, int precision);
};
