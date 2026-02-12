#pragma once

#include <string>

class convert_p10
{
public:
    // Преобразовать число из системы счисления с основанием p в double
    // @param p_num число
    // @param основание системы счисления
    static double Do(const std::string &p_num, int p);

private:
    // Преобразовать символ в число (A = 10, B = 11, C = 12 ...)
    static int charToNum(char ch);

    // Преобразовать строку без разделителя
    // @param digits число
    // @param p основание системы счисления
    // @param weight вес старшего разряда целой части
    static double convert(const std::string &digits, int p, double weight);
};
