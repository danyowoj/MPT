#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>

// Подключаем заголовки тестируемых классов
#include "Editor.h"
#include "Convert_10p.h"
#include "Convert_p10.h"

// ---------------------------------------------------------------------
// Глобальная статистика тестов
// ---------------------------------------------------------------------
static int totalTests = 0;
static int passedTests = 0;
static bool showPassed = true; // можно отключить для краткости

static void verify(bool condition, const char *testName)
{
    totalTests++;
    if (condition)
    {
        passedTests++;
        if (showPassed)
            std::cout << "[PASS] " << testName << "\n";
    }
    else
    {
        std::cout << "[FAIL] " << testName << "\n";
    }
}

static void printSummary(const std::string &groupName)
{
    std::cout << "\n--- " << groupName << " summary: "
              << passedTests << " / " << totalTests << " passed ---\n\n";
}

// ---------------------------------------------------------------------
// Тесты для класса Editor
// ---------------------------------------------------------------------
void testEditor()
{
    std::cout << "===== Testing Editor =====\n";
    totalTests = 0;
    passedTests = 0;

    // ---------- constructor ----------
    {
        Editor ed(10);
        verify(ed.getValue() == "0", "Constructor: initial '0'");
    }

    // ---------- setBase ----------
    {
        Editor ed(10);
        ed.setBase(16);
        ed.addDigit('F');
        verify(ed.getValue() == "F", "setBase(16): allows 'F'");
        ed.setBase(2);
        ed.addDigit('2');
        verify(ed.getValue() == "F", "setBase(2): rejects '2'");
        ed.setBase(1); // должно игнорироваться
        ed.addDigit('1');
        verify(ed.getValue() == "F1", "setBase(1): ignored, '1' added");
        ed.setBase(17); // должно игнорироваться
        ed.addDigit('A');
        verify(ed.getValue() == "F1A", "setBase(17): ignored, 'A' added");
    }

    // ---------- addDigit ----------
    {
        Editor ed(10);
        ed.addDigit('3');
        verify(ed.getValue() == "3", "addDigit: replace initial '0'");
        ed.addDigit('7');
        verify(ed.getValue() == "37", "addDigit: append");
        ed.addDigit('A');
        verify(ed.getValue() == "37", "addDigit: invalid digit ignored");
        Editor ed2(10);
        ed2.addDigit('0');
        verify(ed2.getValue() == "0", "addDigit: '0' on initial stays '0'");
        ed2.addDigit('5');
        verify(ed2.getValue() == "5", "addDigit: after '0' works");
    }

    // ---------- addPoint ----------
    {
        Editor ed(10);
        ed.addPoint();
        verify(ed.getValue() == "0.", "addPoint: first point");
        ed.addPoint();
        verify(ed.getValue() == "0.", "addPoint: second point ignored");
        Editor ed2(10);
        ed2.addDigit('1');
        ed2.addDigit('2');
        ed2.addPoint();
        verify(ed2.getValue() == "12.", "addPoint after integer");
        ed2.addDigit('5');
        verify(ed2.getValue() == "12.5", "addDigit after point");
    }

    // ---------- toggleSign ----------
    {
        Editor ed(10);
        ed.toggleSign();
        verify(ed.getValue() == "-0", "toggleSign: zero -> '-0'");
        ed.toggleSign();
        verify(ed.getValue() == "0", "toggleSign: back to '0'");
        Editor ed2(10);
        ed2.addDigit('1');
        ed2.addDigit('2');
        ed2.toggleSign();
        verify(ed2.getValue() == "-12", "positive -> negative");
        ed2.toggleSign();
        verify(ed2.getValue() == "12", "negative -> positive");
    }

    // ---------- backspace ----------
    {
        Editor ed(10);
        ed.backspace();
        verify(ed.getValue() == "0", "backspace on '0' stays '0'");
        ed.addDigit('5');
        ed.backspace();
        verify(ed.getValue() == "0", "backspace last digit -> '0'");
        ed.addDigit('1');
        ed.addDigit('2');
        ed.addDigit('3');
        ed.backspace();
        verify(ed.getValue() == "12", "backspace removes last digit");
        ed.backspace();
        verify(ed.getValue() == "1", "...");
        ed.backspace();
        verify(ed.getValue() == "0", "...");
        Editor ed2(10);
        ed2.toggleSign();
        ed2.addDigit('7');
        ed2.backspace();
        verify(ed2.getValue() == "-", "backspace digit after sign");
        ed2.backspace();
        verify(ed2.getValue() == "0", "backspace single '-' -> '0'");
    }

    // ---------- clear ----------
    {
        Editor ed(10);
        ed.addDigit('9');
        ed.addPoint();
        ed.addDigit('5');
        ed.toggleSign();
        ed.clear();
        verify(ed.getValue() == "0", "clear resets to '0'");
        ed.addDigit('1');
        verify(ed.getValue() == "1", "clear: subsequent input works");
    }

    // ---------- edge cases (bases) ----------
    {
        Editor ed(2);
        ed.addDigit('1');
        ed.addDigit('0');
        ed.addDigit('1');
        verify(ed.getValue() == "101", "base2: valid digits");
        ed.addDigit('2');
        verify(ed.getValue() == "101", "base2: reject '2'");
        ed.addPoint();
        ed.addDigit('1');
        verify(ed.getValue() == "101.1", "base2: fractional");

        Editor ed2(16);
        ed2.addDigit('F');
        ed2.addDigit('E');
        ed2.addDigit('D');
        verify(ed2.getValue() == "FED", "base16: uppercase");
        ed2.addDigit('c');
        verify(ed2.getValue() == "FED", "base16: lowercase not accepted");

        Editor ed3(16);
        ed3.addDigit('F');
        verify(ed3.getValue() == "F", "base16: max digit");
        ed3.setBase(15);
        ed3.addDigit('F');
        verify(ed3.getValue() == "F", "base15: digit F (15) rejected");
    }

    printSummary("Editor");
}

// ---------------------------------------------------------------------
// Тесты для Convert_10p (десятичная → P-ичная)
// ---------------------------------------------------------------------
void testConvert10p()
{
    std::cout << "===== Testing Convert_10p =====\n";
    totalTests = 0;
    passedTests = 0;

    Convert_10p conv;

    // --- целые числа ---
    verify(conv.Do(10.0, 2, 0) == "1010", "Dec 10 -> bin 1010");
    verify(conv.Do(255.0, 8, 0) == "377", "Dec 255 -> oct 377");
    verify(conv.Do(255.0, 16, 0) == "FF", "Dec 255 -> hex FF");
    verify(conv.Do(0.0, 16, 0) == "0", "Dec 0 -> 0");
    verify(conv.Do(123.0, 10, 0) == "123", "Dec 123 -> dec 123");

    // --- дробные числа ---
    verify(conv.Do(10.625, 2, 3) == "1010.101", "Dec 10.625 -> bin 1010.101 (prec 3)");
    verify(conv.Do(123.456, 10, 3) == "123.456", "Dec 123.456 -> dec 123.456");
    verify(conv.Do(0.5, 2, 5) == "0.10000", "Dec 0.5 -> bin 0.1 (prec 5)");

    // --- отрицательные ---
    verify(conv.Do(-123.0, 10, 0) == "-123", "Negative -> '-' prefix");
    verify(conv.Do(-10.625, 2, 3) == "-1010.101", "Negative fraction");

    // --- граничные условия ---
    verify(conv.Do(123.0, 1, 0) == "0", "Base 1 (invalid) -> 0");
    verify(conv.Do(123.0, 17, 0) == "0", "Base 17 (invalid) -> 0");
    verify(conv.Do(123.0, 10, -1) == "0", "Negative precision -> 0");

    printSummary("Convert_10p");
}

// ---------------------------------------------------------------------
// Тесты для Convert_p10 (P-ичная → десятичная)
// ---------------------------------------------------------------------
void testConvertP10()
{
    std::cout << "===== Testing Convert_p10 =====\n";
    totalTests = 0;
    passedTests = 0;

    Convert_p10 conv;

    // --- целые числа ---
    verify(std::fabs(conv.Do("1010", 2) - 10.0) < 1e-9, "Bin 1010 -> 10");
    verify(std::fabs(conv.Do("777", 8) - 511.0) < 1e-9, "Oct 777 -> 511");
    verify(std::fabs(conv.Do("FF", 16) - 255.0) < 1e-9, "Hex FF -> 255");
    verify(std::fabs(conv.Do("abc", 16) - 2748.0) < 1e-9, "Hex abc -> 2748 (lowercase)");
    verify(std::fabs(conv.Do("0", 10) - 0.0) < 1e-9, "'0' -> 0");

    // --- дробные числа ---
    verify(std::fabs(conv.Do("1010.101", 2) - 10.625) < 1e-9, "Bin 1010.101 -> 10.625");
    verify(std::fabs(conv.Do("123.456", 10) - 123.456) < 1e-9, "Dec 123.456 -> 123.456");
    verify(std::fabs(conv.Do("0.1", 2) - 0.5) < 1e-9, "Bin 0.1 -> 0.5");

    // --- отрицательные ---
    verify(std::fabs(conv.Do("-123", 10) + 123.0) < 1e-9, "Negative -123 -> -123");
    verify(std::fabs(conv.Do("-1010.101", 2) + 10.625) < 1e-9, "Negative binary fraction");

    // --- граничные условия ---
    verify(std::fabs(conv.Do("", 10)) < 1e-9, "Empty string -> 0");
    verify(std::fabs(conv.Do("123", 1)) < 1e-9, "Base 1 -> 0");
    verify(std::fabs(conv.Do("123", 17)) < 1e-9, "Base 17 -> 0");
    verify(std::fabs(conv.Do("12G", 16)) < 1e-9, "Invalid digit G -> 0");
    verify(std::fabs(conv.Do("12.34.56", 10)) < 1e-9, "Two dots -> 0? (точка найдётся первая, остальное в дробную часть – ожидаем 0 из-за второй точки? По коду: вторая точка попадёт в дробную часть и вызовет ошибку конвертации -> вернёт 0)");

    printSummary("Convert_p10");
}

// ---------------------------------------------------------------------
// Тесты round-trip: совместная работа обоих конвертеров
// ---------------------------------------------------------------------
void testRoundTrip()
{
    std::cout << "===== Testing Converters Round-Trip =====\n";
    totalTests = 0;
    passedTests = 0;

    Convert_10p toP;
    Convert_p10 to10;

    double values[] = {0, 1, 42, 255, 3.14, 10.625, -123.456};
    int bases[] = {2, 8, 10, 16};
    int precisions[] = {0, 3, 5};

    for (double v : values)
    {
        for (int p : bases)
        {
            for (int prec : precisions)
            {
                std::string pStr = toP.Do(v, p, prec);
                double back = to10.Do(pStr, p);
                // Сравнение с учётом точности преобразования
                double tolerance = (prec == 0) ? 1e-9 : std::pow(p, -prec) * 1.1;
                bool ok = std::fabs(back - v) < tolerance;
                if (!ok)
                {
                    std::cout << "Round-trip failed: " << v << " -> " << pStr
                              << " -> " << back << " (base " << p << ", prec " << prec << ")\n";
                }
                // Для краткости выводим только ошибки, но статистику собираем
                totalTests++;
                if (ok)
                    passedTests++;
            }
        }
    }

    printSummary("Converters Round-Trip");
}

// ---------------------------------------------------------------------
// Главная функция – разбор аргументов командной строки
// ---------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // По умолчанию тестируем всё
    bool testEd = false, testC10p = false, testCp10 = false, testRT = false;
    bool runAll = true;

    if (argc > 1)
    {
        runAll = false;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == "--editor" || arg == "-e")
                testEd = true;
            else if (arg == "--convert10p" || arg == "-c10")
                testC10p = true;
            else if (arg == "--convertp10" || arg == "-cp10")
                testCp10 = true;
            else if (arg == "--roundtrip" || arg == "-rt")
                testRT = true;
            else if (arg == "--all" || arg == "-a")
                runAll = true;
            else if (arg == "--quiet")
                showPassed = false; // не показывать успешные тесты
            else
            {
                std::cerr << "Unknown option: " << arg << "\n";
                return 1;
            }
        }
    }

    if (runAll)
    {
        testEd = testC10p = testCp10 = testRT = true;
    }

    // Сброс статистики перед каждым тестом выполняется внутри них,
    // но итоговую общую статистику соберём вручную.
    int grandTotal = 0, grandPassed = 0;

    if (testEd)
    {
        testEditor();
        grandTotal += totalTests;
        grandPassed += passedTests;
    }
    if (testC10p)
    {
        testConvert10p();
        grandTotal += totalTests;
        grandPassed += passedTests;
    }
    if (testCp10)
    {
        testConvertP10();
        grandTotal += totalTests;
        grandPassed += passedTests;
    }
    if (testRT)
    {
        testRoundTrip();
        grandTotal += totalTests;
        grandPassed += passedTests;
    }

    std::cout << "\n========================================\n";
    std::cout << "TOTAL PASSED: " << grandPassed << " / " << grandTotal << "\n";
    std::cout << "========================================\n";

    return (grandPassed == grandTotal) ? 0 : 1;
}
