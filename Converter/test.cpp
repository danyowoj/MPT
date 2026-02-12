// test_convert.cpp
#include <iostream>
#include <string>
#include <cmath>

// Подключаем заголовочные файлы
#include "convert_p10.h"
#include "convert_10p.h"

// Функция для сравнения чисел с плавающей точкой с заданной точностью
bool almostEqual(double a, double b, double epsilon = 1e-9)
{
    return std::fabs(a - b) < epsilon;
}

// Тесты для convert_p10 (из P-ичной в десятичную)
void testConvertP10()
{
    std::cout << "=== Тестирование convert_p10 ===\n";

    convert_p10 converter;
    int passed = 0;
    int total = 0;

    // Тест 1: Двоичная система
    {
        total++;
        double result = converter.Do("1010", 2);
        if (almostEqual(result, 10.0))
        {
            std::cout << "Тест 1 passed: 1010 (2) -> " << result << "\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 1 failed: 1010 (2) ожидается 10, получено " << result << "\n";
        }
    }

    // Тест 2: Восьмеричная система
    {
        total++;
        double result = converter.Do("777", 8);
        if (almostEqual(result, 511.0))
        {
            std::cout << "Тест 2 passed: 777 (8) -> " << result << "\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 2 failed: 777 (8) ожидается 511, получено " << result << "\n";
        }
    }

    // Тест 3: Шестнадцатеричная система (верхний регистр)
    {
        total++;
        double result = converter.Do("FF", 16);
        if (almostEqual(result, 255.0))
        {
            std::cout << "Тест 3 passed: FF (16) -> " << result << "\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 3 failed: FF (16) ожидается 255, получено " << result << "\n";
        }
    }

    // Тест 4: Шестнадцатеричная система (нижний регистр)
    {
        total++;
        double result = converter.Do("abc", 16);
        if (almostEqual(result, 2748.0))
        {
            std::cout << "Тест 4 passed: abc (16) -> " << result << "\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 4 failed: abc (16) ожидается 2748, получено " << result << "\n";
        }
    }

    // Тест 5: Десятичная система с дробной частью
    {
        total++;
        double result = converter.Do("123.456", 10);
        if (almostEqual(result, 123.456))
        {
            std::cout << "Тест 5 passed: 123.456 (10) -> " << result << "\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 5 failed: 123.456 (10) ожидается 123.456, получено " << result << "\n";
        }
    }

    // Тест 6: Двоичная система с дробной частью
    {
        total++;
        double result = converter.Do("1010.101", 2);
        if (almostEqual(result, 10.625))
        {
            std::cout << "Тест 6 passed: 1010.101 (2) -> " << result << "\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 6 failed: 1010.101 (2) ожидается 10.625, получено " << result << "\n";
        }
    }

    // Тест 7: Отрицательные числа
    {
        total++;
        double result = converter.Do("-123", 10);
        if (almostEqual(result, -123.0))
        {
            std::cout << "Тест 7 passed: -123 (10) -> " << result << "\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 7 failed: -123 (10) ожидается -123, получено " << result << "\n";
        }
    }

    // Тест 8: Некорректное основание (должно вернуть 0)
    {
        total++;
        double result = converter.Do("123", 1); // Основание < 2
        if (almostEqual(result, 0.0))
        {
            std::cout << "Тест 8 passed: 123 (1) -> " << result << " (некорректное основание)\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 8 failed: 123 (1) ожидается 0, получено " << result << "\n";
        }
    }

    // Тест 9: Пустая строка (должно вернуть 0)
    {
        total++;
        double result = converter.Do("", 10);
        if (almostEqual(result, 0.0))
        {
            std::cout << "Тест 9 passed: пустая строка -> " << result << "\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 9 failed: пустая строка ожидается 0, получено " << result << "\n";
        }
    }

    std::cout << "Результат: " << passed << "/" << total << " тестов пройдено\n\n";
}

// Тесты для convert_10p (из десятичной в P-ичную)
void testConvert10P()
{
    std::cout << "=== Тестирование convert_10p ===\n";

    convert_10p converter;
    int passed = 0;
    int total = 0;

    // Тест 1: Десятичное в двоичное (целое)
    {
        total++;
        std::string result = converter.Do(10.0, 2, 0);
        if (result == "1010")
        {
            std::cout << "Тест 1 passed: 10 (10) -> " << result << " (2)\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 1 failed: 10 (10) ожидается 1010, получено " << result << "\n";
        }
    }

    // Тест 2: Десятичное в восьмеричное (целое)
    {
        total++;
        std::string result = converter.Do(255.0, 8, 0);
        if (result == "377")
        {
            std::cout << "Тест 2 passed: 255 (10) -> " << result << " (8)\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 2 failed: 255 (10) ожидается 377, получено " << result << "\n";
        }
    }

    // Тест 3: Десятичное в шестнадцатеричное (целое)
    {
        total++;
        std::string result = converter.Do(255.0, 16, 0);
        if (result == "FF")
        {
            std::cout << "Тест 3 passed: 255 (10) -> " << result << " (16)\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 3 failed: 255 (10) ожидается FF, получено " << result << "\n";
        }
    }

    // Тест 4: Десятичное в двоичное (дробное, точность 3)
    {
        total++;
        std::string result = converter.Do(10.625, 2, 3);
        if (result == "1010.101")
        {
            std::cout << "Тест 4 passed: 10.625 (10) -> " << result << " (2, точность 3)\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 4 failed: 10.625 (10) ожидается 1010.101, получено " << result << "\n";
        }
    }

    // Тест 5: Десятичное в десятичное (дробное, точность 3)
    {
        total++;
        std::string result = converter.Do(123.456, 10, 3);
        if (result == "123.456")
        {
            std::cout << "Тест 5 passed: 123.456 (10) -> " << result << " (10, точность 3)\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 5 failed: 123.456 (10) ожидается 123.456, получено " << result << "\n";
        }
    }

    // Тест 6: Отрицательное число
    {
        total++;
        std::string result = converter.Do(-123.0, 10, 0);
        if (result == "-123")
        {
            std::cout << "Тест 6 passed: -123 (10) -> " << result << " (10)\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 6 failed: -123 (10) ожидается -123, получено " << result << "\n";
        }
    }

    // Тест 7: Ноль
    {
        total++;
        std::string result = converter.Do(0.0, 2, 0);
        if (result == "0")
        {
            std::cout << "Тест 7 passed: 0 (10) -> " << result << " (2)\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 7 failed: 0 (10) ожидается 0, получено " << result << "\n";
        }
    }

    // Тест 8: Некорректное основание (должно вернуть "0")
    {
        total++;
        std::string result = converter.Do(123.0, 1, 0); // Основание < 2
        if (result == "0")
        {
            std::cout << "Тест 8 passed: 123 (10) -> " << result << " (некорректное основание)\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 8 failed: 123 (10) ожидается 0, получено " << result << "\n";
        }
    }

    // Тест 9: Отрицательная точность (должно вернуть "0")
    {
        total++;
        std::string result = converter.Do(123.0, 10, -1);
        if (result == "0")
        {
            std::cout << "Тест 9 passed: точность -1 -> " << result << "\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 9 failed: точность -1 ожидается 0, получено " << result << "\n";
        }
    }

    // Тест 10: Большое число с дробной частью
    {
        total++;
        std::string result = converter.Do(123456.789, 10, 3);
        if (result == "123456.789")
        {
            std::cout << "Тест 10 passed: 123456.789 (10) -> " << result << " (10, точность 3)\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 10 failed: 123456.789 (10) ожидается 123456.789, получено " << result << "\n";
        }
    }

    std::cout << "Результат: " << passed << "/" << total << " тестов пройдено\n\n";
}

// Тесты на совместимость: преобразование туда и обратно
void testRoundTrip()
{
    std::cout << "=== Тестирование round-trip (туда и обратно) ===\n";

    convert_p10 to10;
    convert_10p toP;
    int passed = 0;
    int total = 0;

    // Тест 1: Целое число из 10 в 16 и обратно
    {
        total++;
        std::string p_result = toP.Do(255.0, 16, 0);
        double back_result = to10.Do(p_result, 16);
        if (almostEqual(back_result, 255.0))
        {
            std::cout << "Тест 1 passed: 255 -> " << p_result << " -> " << back_result << "\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 1 failed: 255 -> " << p_result << " -> " << back_result << "\n";
        }
    }

    // Тест 2: Дробное число из 10 в 2 и обратно (с точностью)
    {
        total++;
        std::string p_result = toP.Do(10.625, 2, 3);
        double back_result = to10.Do(p_result, 2);
        if (almostEqual(back_result, 10.625))
        {
            std::cout << "Тест 2 passed: 10.625 -> " << p_result << " -> " << back_result << "\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 2 failed: 10.625 -> " << p_result << " -> " << back_result << "\n";
        }
    }

    // Тест 3: Отрицательное число
    {
        total++;
        std::string p_result = toP.Do(-123.0, 10, 0);
        double back_result = to10.Do(p_result, 10);
        if (almostEqual(back_result, -123.0))
        {
            std::cout << "Тест 3 passed: -123 -> " << p_result << " -> " << back_result << "\n";
            passed++;
        }
        else
        {
            std::cout << "Тест 3 failed: -123 -> " << p_result << " -> " << back_result << "\n";
        }
    }

    std::cout << "Результат: " << passed << "/" << total << " тестов пройдено\n\n";
}

int main()
{
    std::cout << "Запуск unit-тестов для преобразования систем счисления\n";
    std::cout << "=====================================================\n\n";

    testConvertP10();
    testConvert10P();
    testRoundTrip();

    std::cout << "Все тесты завершены!\n";

    return 0;
}
