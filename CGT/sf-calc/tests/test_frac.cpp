#include "../core/UFrac.h"
#include <iostream>
#include <string>
#include <cmath>

// Простая тестовая среда: подсчёт успешных и упавших проверок
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

#define CHECK_THROWS(expr, expected_exception)                          \
    do                                                                  \
    {                                                                   \
        ++total_checks;                                                 \
        bool threw_expected = false;                                    \
        try                                                             \
        {                                                               \
            expr;                                                       \
        }                                                               \
        catch (const expected_exception &)                              \
        {                                                               \
            threw_expected = true;                                      \
        }                                                               \
        catch (...)                                                     \
        {                                                               \
        }                                                               \
        if (!threw_expected)                                            \
        {                                                               \
            ++failed_checks;                                            \
            std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__        \
                      << " – ожидалось исключение " #expected_exception \
                      << " в " << #expr << std::endl;                   \
        }                                                               \
    } while (false)

#define CHECK_NOTHROW(expr)                                                     \
    do                                                                          \
    {                                                                           \
        ++total_checks;                                                         \
        try                                                                     \
        {                                                                       \
            expr;                                                               \
        }                                                                       \
        catch (...)                                                             \
        {                                                                       \
            ++failed_checks;                                                    \
            std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__                \
                      << " – не ожидалось исключение в " << #expr << std::endl; \
        }                                                                       \
    } while (false)

// ------------------------------------------------------------------
// Тесты конструкторов и нормализации
void testConstructors()
{
    std::cout << "\n--- Testing constructors ---\n";

    // По умолчанию
    TFrac f1;
    CHECK(f1.getNumerator() == 0);
    CHECK(f1.getDenominator() == 1);
    CHECK(f1.toString() == "0/1");

    // От двух целых
    TFrac f2(3, 6);
    CHECK(f2.getNumerator() == 1);
    CHECK(f2.getDenominator() == 2);
    CHECK(f2.toString() == "1/2");

    TFrac f3(-4, 8);
    CHECK(f3.getNumerator() == -1);
    CHECK(f3.getDenominator() == 2);
    CHECK(f3.toString() == "-1/2");

    TFrac f4(0, 5);
    CHECK(f4.getNumerator() == 0);
    CHECK(f4.getDenominator() == 1);
    CHECK(f4.toString() == "0/1");

    // Знак переносится
    TFrac f5(3, -4);
    CHECK(f5.getNumerator() == -3);
    CHECK(f5.getDenominator() == 4);
    CHECK(f5.toString() == "-3/4");

    // Конструктор от строки (целое)
    TFrac f6("5");
    CHECK(f6.getNumerator() == 5);
    CHECK(f6.getDenominator() == 1);
    CHECK(f6.toString() == "5/1");

    TFrac f7("-3");
    CHECK(f7.getNumerator() == -3);
    CHECK(f7.getDenominator() == 1);

    // Простая дробь
    TFrac f8("2/3");
    CHECK(f8.getNumerator() == 2);
    CHECK(f8.getDenominator() == 3);

    TFrac f9("-4/6");
    CHECK(f9.getNumerator() == -2);
    CHECK(f9.getDenominator() == 3);

    // Смешанная дробь
    TFrac f10("2|1/3"); // 2 + 1/3 = 7/3
    CHECK(f10.getNumerator() == 7);
    CHECK(f10.getDenominator() == 3);

    TFrac f11("-2|1/3"); // -2 + 1/3 = -5/3
    CHECK(f11.getNumerator() == -5);
    CHECK(f11.getDenominator() == 3);

    TFrac f12("2|-1/3"); // 2 - 1/3 = 5/3
    CHECK(f12.getNumerator() == 5);
    CHECK(f12.getDenominator() == 3);

    TFrac f13("-2|-1/3"); // -2 - 1/3 = -7/3
    CHECK(f13.getNumerator() == -7);
    CHECK(f13.getDenominator() == 3);

    // Конструктор копирования (неявно)
    TFrac f14 = f10;
    CHECK(f14.equals(f10));

    // Проверка исключений
    CHECK_THROWS(TFrac(1, 0), std::invalid_argument);
    CHECK_THROWS(TFrac("5/0"), std::invalid_argument);
    CHECK_THROWS(TFrac("2|a/3"), std::invalid_argument);
    CHECK_THROWS(TFrac("abc"), std::invalid_argument);
}

// ------------------------------------------------------------------
// Тесты арифметических операций
void testArithmetic()
{
    std::cout << "\n--- Testing arithmetic ---\n";

    TFrac a(1, 2);
    TFrac b(1, 3);

    TFrac sum = a.add(b);
    CHECK(sum.getNumerator() == 5);
    CHECK(sum.getDenominator() == 6);

    TFrac diff = a.subtract(b);
    CHECK(diff.getNumerator() == 1);
    CHECK(diff.getDenominator() == 6);

    TFrac prod = a.multiply(b);
    CHECK(prod.getNumerator() == 1);
    CHECK(prod.getDenominator() == 6);

    TFrac quot = a.divide(b);
    CHECK(quot.getNumerator() == 3);
    CHECK(quot.getDenominator() == 2); // 1/2 : 1/3 = 3/2

    TFrac sq = a.square();
    CHECK(sq.getNumerator() == 1);
    CHECK(sq.getDenominator() == 4);

    TFrac inv = a.reciprocal();
    CHECK(inv.getNumerator() == 2);
    CHECK(inv.getDenominator() == 1);

    TFrac neg = a.negate();
    CHECK(neg.getNumerator() == -1);
    CHECK(neg.getDenominator() == 2);

    // Проверка исключений в арифметике
    TFrac zero(0, 1);
    CHECK_THROWS(a.divide(zero), std::invalid_argument);
    CHECK_THROWS(zero.reciprocal(), std::invalid_argument);
}

// ------------------------------------------------------------------
// Тесты сравнения
void testComparison()
{
    std::cout << "\n--- Testing comparison ---\n";

    TFrac c1(2, 4); // 1/2
    TFrac c2(1, 2);
    CHECK(c1.equals(c2));
    CHECK(c1.greaterThan(TFrac(1, 3))); // 1/2 > 1/3
    CHECK(!TFrac(1, 3).greaterThan(c1));

    TFrac c3(-1, 2);
    TFrac c4(1, -2); // после нормализации -1/2
    CHECK(c3.equals(c4));
}

// ------------------------------------------------------------------
// Тесты геттеров
void testGetters()
{
    std::cout << "\n--- Testing getters ---\n";

    TFrac f(1, 2);
    CHECK(f.getNumerator() == 1);
    CHECK(f.getDenominator() == 2);
    CHECK(f.getNumeratorString() == "1");
    CHECK(f.getDenominatorString() == "2");
    CHECK(f.toString() == "1/2");
}

// ------------------------------------------------------------------
// Тесты дополнительных методов (toDouble, isTerminatingDecimal)
void testExtras()
{
    std::cout << "\n--- Testing extra methods ---\n";

    TFrac d1(1, 2);
    CHECK(d1.toDouble() == 0.5);
    CHECK(d1.isTerminatingDecimal() == true);

    TFrac d2(1, 3);
    double val = d2.toDouble();
    CHECK(std::abs(val - 0.3333333333) < 1e-9);
    CHECK(d2.isTerminatingDecimal() == false);

    TFrac d3(3, 8);                           // 0.375
    CHECK(d3.isTerminatingDecimal() == true); // 8=2^3

    TFrac d4(7, 20); // 20=2^2*5
    CHECK(d4.isTerminatingDecimal() == true);

    TFrac d5(7, 15); // 15=3*5
    CHECK(d5.isTerminatingDecimal() == false);
}

// ------------------------------------------------------------------
int main()
{
    std::cout << "Starting TFrac tests...\n";

    testConstructors();
    testArithmetic();
    testComparison();
    testGetters();
    testExtras();

    std::cout << "\n========================================\n";
    std::cout << "Total checks: " << total_checks << "\n";
    std::cout << "Failed checks: " << failed_checks << "\n";
    if (failed_checks == 0)
        std::cout << "ALL TESTS PASSED!\n";
    else
        std::cout << "SOME TESTS FAILED!\n";
    std::cout << "========================================\n";

    return (failed_checks == 0) ? 0 : 1;
}
