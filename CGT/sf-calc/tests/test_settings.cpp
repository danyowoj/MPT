#include <windows.h>

#include "../include/TSettings.h"
#include <iostream>
#include <cstdio> // для remove

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

void testDefault()
{
    std::cout << "\n--- TSettings default ---\n";
    TSettings s;
    CHECK(s.displayFormat() == DisplayFormat::Fraction);
    CHECK(s.operandSource() == OperandSource::Memory);
}

void testSetters()
{
    std::cout << "\n--- TSettings setters ---\n";
    TSettings s;
    s.setDisplayFormat(DisplayFormat::Decimal);
    s.setOperandSource(OperandSource::Clipboard);
    CHECK(s.displayFormat() == DisplayFormat::Decimal);
    CHECK(s.operandSource() == OperandSource::Clipboard);
}

void testSaveLoad()
{
    std::cout << "\n--- TSettings save/load ---\n";
    const char *filename = "test_settings.cfg";

    TSettings s1;
    s1.setDisplayFormat(DisplayFormat::Decimal);
    s1.setOperandSource(OperandSource::Clipboard);
    s1.saveToFile(filename);

    TSettings s2; // по умолчанию Fraction/Memory
    s2.loadFromFile(filename);
    CHECK(s2.displayFormat() == DisplayFormat::Decimal);
    CHECK(s2.operandSource() == OperandSource::Clipboard);

    // Проверка на несуществующий файл (должен оставить настройки без изменений)
    TSettings s3;
    s3.loadFromFile("nonexistent.cfg");
    CHECK(s3.displayFormat() == DisplayFormat::Fraction);
    CHECK(s3.operandSource() == OperandSource::Memory);

    std::remove(filename);
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "Starting TSettings tests...\n";
    testDefault();
    testSetters();
    testSaveLoad();

    std::cout << "\n========================================\n";
    std::cout << "Total checks: " << total_checks << "\n";
    std::cout << "Failed checks: " << failed_checks << "\n";
    if (failed_checks == 0)
        std::cout << "ALL TESTS PASSED!\n";
    else
        std::cout << "SOME TESTS FAILED!\n";
    return (failed_checks == 0) ? 0 : 1;
}
