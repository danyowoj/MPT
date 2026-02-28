#include <windows.h>

#include "../include/UEditor.h"
#include "../include/UFrac.h"
#include <iostream>

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

// ------------------------------------------------------------------
void testInitialState()
{
    std::cout << "\n--- TEditor initial state ---\n";
    TEditor ed;
    CHECK(ed.getString() == "0/1");
    CHECK(ed.isZero() == true);
}

void testClear()
{
    std::cout << "\n--- TEditor clear ---\n";
    TEditor ed;
    ed.addDigit(5);
    ed.addDigit(3);
    CHECK(ed.getString() != "0/1");
    ed.clear();
    CHECK(ed.getString() == "0/1");
    CHECK(ed.isZero() == true);
}

void testAddDigit()
{
    std::cout << "\n--- TEditor addDigit ---\n";
    TEditor ed;
    ed.addDigit(5);
    // "0/1" + '5' = "0/15" (если валидно)
    CHECK(ed.getString() == "0/15");
    ed.addDigit(3);
    CHECK(ed.getString() == "0/153");
    ed.addDigit(10); // не цифра
    CHECK(ed.getString() == "0/153");
}

void testAddZero()
{
    std::cout << "\n--- TEditor addZero ---\n";
    TEditor ed;
    ed.addZero();
    CHECK(ed.getString() == "0/10");
}

void testToggleSign()
{
    std::cout << "\n--- TEditor toggleSign ---\n";
    TEditor ed;
    ed.toggleSign();
    CHECK(ed.getString() == "-0/1");
    ed.toggleSign();
    CHECK(ed.getString() == "0/1");

    // Проверка на пустой строке (setString устанавливает "0/1")
    TEditor ed2;
    ed2.setString("");
    ed2.toggleSign();
    CHECK(ed2.getString() == "-0/1");
}

void testBackspace()
{
    std::cout << "\n--- TEditor backspace ---\n";
    TEditor ed;
    ed.addDigit(1);
    ed.addDigit(2);
    ed.addDigit(3);
    CHECK(ed.getString() == "0/1123");
    ed.backspace();
    CHECK(ed.getString() == "0/112");
    ed.backspace();
    CHECK(ed.getString() == "0/11");
    ed.backspace();
    CHECK(ed.getString() == "0/1"); // после удаления до "0/1"
}

void testSetGet()
{
    std::cout << "\n--- TEditor setString/getString ---\n";
    TEditor ed;
    ed.setString("5/8");
    CHECK(ed.getString() == "5/8");
    CHECK(ed.isZero() == false);
    ed.setString("");
    CHECK(ed.getString() == "0/1");
}

void testEdit()
{
    std::cout << "\n--- TEditor edit ---\n";
    TEditor ed;
    ed.edit(7);
    CHECK(ed.getString() == "0/17");
    ed.edit(-1); // переключить знак
    CHECK(ed.getString() == "-0/17");
    ed.edit(-2); // backspace
    CHECK(ed.getString() == "-0/1");
    ed.edit(-3); // clear
    CHECK(ed.getString() == "0/1");
    ed.edit(100); // неизвестная команда
    CHECK(ed.getString() == "0/1");
}

void testValidation()
{
    std::cout << "\n--- TEditor validation ---\n";
    TEditor ed;
}

// ------------------------------------------------------------------
int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "Starting TEditor tests...\n";
    testInitialState();
    testClear();
    testAddDigit();
    testAddZero();
    testToggleSign();
    testBackspace();
    testSetGet();
    testEdit();
    testValidation();

    std::cout << "\n========================================\n";
    std::cout << "Total checks: " << total_checks << "\n";
    std::cout << "Failed checks: " << failed_checks << "\n";
    if (failed_checks == 0)
        std::cout << "ALL TESTS PASSED!\n";
    else
        std::cout << "SOME TESTS FAILED!\n";
    return (failed_checks == 0) ? 0 : 1;
}
