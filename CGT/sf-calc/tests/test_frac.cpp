#include <windows.h>

#include "../include/UFrac.h"
#include <iostream>
#include <cmath>

static int total_checks = 0;
static int failed_checks = 0;

#define CHECK(condition)                                         \
    do                                                           \
    {                                                            \
        ++total_checks;                                          \
        if (!(condition))                                        \
        {                                                        \
            ++failed_checks;                                     \
            std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__ \
                      << " – " << #condition << std::endl;       \
        }                                                        \
    } while (false)

#define CHECK_THROWS(expr, exception_type)                                                    \
    do                                                                                        \
    {                                                                                         \
        ++total_checks;                                                                       \
        try                                                                                   \
        {                                                                                     \
            expr;                                                                             \
            ++failed_checks;                                                                  \
            std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__                              \
                      << " – " << #expr << " did not throw " << #exception_type << std::endl; \
        }                                                                                     \
        catch (const exception_type &)                                                        \
        {                                                                                     \
            /* expected */                                                                    \
        }                                                                                     \
        catch (...)                                                                           \
        {                                                                                     \
            ++failed_checks;                                                                  \
            std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__                              \
                      << " – " << #expr << " threw a different exception" << std::endl;       \
        }                                                                                     \
    } while (false)

// ------------------------------------------------------------------
void testConstructors()
{
    std::cout << "\n--- TFrac constructors ---\n";

    // По умолчанию
    TFrac f1;
    CHECK(f1.numerator() == 0);
    CHECK(f1.denominator() == 1);
    CHECK(f1.toString() == "0/1");

    // От двух целых
    TFrac f2(3, 6);
    CHECK(f2.numerator() == 1);
    CHECK(f2.denominator() == 2);
    CHECK(f2.toString() == "1/2");

    TFrac f3(-4, 8);
    CHECK(f3.numerator() == -1);
    CHECK(f3.denominator() == 2);
    CHECK(f3.toString() == "-1/2");

    TFrac f4(0, 5);
    CHECK(f4.numerator() == 0);
    CHECK(f4.denominator() == 1);
    CHECK(f4.toString() == "0/1");

    // Знак переносится
    TFrac f5(3, -4);
    CHECK(f5.numerator() == -3);
    CHECK(f5.denominator() == 4);
    CHECK(f5.toString() == "-3/4");

    // Из строки (целое)
    TFrac f6("5");
    CHECK(f6.numerator() == 5);
    CHECK(f6.denominator() == 1);
    CHECK(f6.toString() == "5/1");

    TFrac f7("-3");
    CHECK(f7.numerator() == -3);
    CHECK(f7.denominator() == 1);

    // Простая дробь
    TFrac f8("2/3");
    CHECK(f8.numerator() == 2);
    CHECK(f8.denominator() == 3);

    TFrac f9("-4/6");
    CHECK(f9.numerator() == -2);
    CHECK(f9.denominator() == 3);

    // Смешанная дробь
    TFrac f10("2|1/3"); // 2 + 1/3 = 7/3
    CHECK(f10.numerator() == 7);
    CHECK(f10.denominator() == 3);

    TFrac f11("-2|1/3"); // -2 + 1/3 = -5/3 (минус перед целой частью относится ко всему числу, но в нашей интерпретации -2 + 1/3 = -5/3)
    CHECK(f11.numerator() == -5);
    CHECK(f11.denominator() == 3);

    TFrac f12("2|-1/3"); // 2 - 1/3 = 5/3
    CHECK(f12.numerator() == 5);
    CHECK(f12.denominator() == 3);

    TFrac f13("-2|-1/3"); // -2 - 1/3 = -7/3
    CHECK(f13.numerator() == -7);
    CHECK(f13.denominator() == 3);

    // Копирование
    TFrac f14 = f10;
    CHECK(f14 == f10);

    // Исключения
    CHECK_THROWS(TFrac(1, 0), std::invalid_argument);
    CHECK_THROWS(TFrac("5/0"), std::invalid_argument);
    CHECK_THROWS(TFrac("2|a/3"), std::invalid_argument);
    CHECK_THROWS(TFrac("abc"), std::invalid_argument);
}

void testNormalize()
{
    std::cout << "\n--- TFrac normalize ---\n";
    TFrac f(2, 4);
    CHECK(f.numerator() == 1);
    CHECK(f.denominator() == 2);
    CHECK(f.toString() == "1/2");
}

void testArithmetic()
{
    std::cout << "\n--- TFrac arithmetic ---\n";
    TFrac a(1, 2);
    TFrac b(1, 3);

    TFrac sum = a + b;
    CHECK(sum.numerator() == 5);
    CHECK(sum.denominator() == 6);

    TFrac diff = a - b;
    CHECK(diff.numerator() == 1);
    CHECK(diff.denominator() == 6);

    TFrac prod = a * b;
    CHECK(prod.numerator() == 1);
    CHECK(prod.denominator() == 6);

    TFrac quot = a / b;
    CHECK(quot.numerator() == 3);
    CHECK(quot.denominator() == 2);

    TFrac sq = a.square();
    CHECK(sq.numerator() == 1);
    CHECK(sq.denominator() == 4);

    TFrac inv = a.reciprocal();
    CHECK(inv.numerator() == 2);
    CHECK(inv.denominator() == 1);

    TFrac neg = a.negate();
    CHECK(neg.numerator() == -1);
    CHECK(neg.denominator() == 2);

    // Деление на ноль
    TFrac zero(0, 1);
    CHECK_THROWS(a / zero, std::invalid_argument);
    CHECK_THROWS(zero.reciprocal(), std::invalid_argument);
}

void testComparison()
{
    std::cout << "\n--- TFrac comparison ---\n";
    TFrac a(1, 2);
    TFrac b(1, 2);
    TFrac c(1, 3);
    TFrac d(-1, 2);

    CHECK(a == b);
    CHECK(!(a != b));
    CHECK(a > c);
    CHECK(c < a);
    CHECK(a >= b);
    CHECK(a <= b);
    CHECK(d < a);
    CHECK(!(d > a));
}

void testGetters()
{
    std::cout << "\n--- TFrac getters ---\n";
    TFrac f(3, 4);
    CHECK(f.numerator() == 3);
    CHECK(f.denominator() == 4);
    CHECK(f.isZero() == false);

    TFrac zero(0, 5);
    CHECK(zero.isZero() == true);
}

void testToString()
{
    std::cout << "\n--- TFrac toString ---\n";
    TFrac f(1, 2);
    CHECK(f.toString() == "1/2");
    CHECK(f.toString(DisplayFormat::Fraction) == "1/2");

    // Десятичный формат
    TFrac f2(3, 8);                                        // 0.375
    CHECK(f2.toString(DisplayFormat::Decimal) == "0.375"); // зависит от реализации

    TFrac f3(1, 3);                                      // не представимо конечной
    CHECK(f3.toString(DisplayFormat::Decimal) == "1/3"); // должно вернуть дробь
}

void testToDouble()
{
    std::cout << "\n--- TFrac toDouble ---\n";
    TFrac f(1, 2);
    CHECK(std::abs(f.toDouble() - 0.5) < 1e-9);
}

void testTerminatingDecimal()
{
    std::cout << "\n--- TFrac isTerminatingDecimal ---\n";
    TFrac f1(1, 2);
    CHECK(f1.isTerminatingDecimal() == true);
    TFrac f2(1, 3);
    CHECK(f2.isTerminatingDecimal() == false);
    TFrac f3(3, 8);
    CHECK(f3.isTerminatingDecimal() == true);
    TFrac f4(7, 20);
    CHECK(f4.isTerminatingDecimal() == true);
    TFrac f5(7, 15);
    CHECK(f5.isTerminatingDecimal() == false);
}

// ------------------------------------------------------------------
int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "Starting TFrac tests...\n";
    testConstructors();
    testNormalize();
    testArithmetic();
    testComparison();
    testGetters();
    testToString();
    testToDouble();
    testTerminatingDecimal();

    std::cout << "\n========================================\n";
    std::cout << "Total checks: " << total_checks << "\n";
    std::cout << "Failed checks: " << failed_checks << "\n";
    if (failed_checks == 0)
        std::cout << "ALL TESTS PASSED!\n";
    else
        std::cout << "SOME TESTS FAILED!\n";
    return (failed_checks == 0) ? 0 : 1;
}
