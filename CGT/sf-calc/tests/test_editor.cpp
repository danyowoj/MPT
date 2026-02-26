#include "../core/UEditor.h"
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
void testEditorConstructors()
{
    std::cout << "\n--- Testing TEditor constructors ---\n";
    TEditor ed;
    CHECK(ed.getString() == "0/1");
    CHECK(ed.isZero() == true);
}

void testEditorClear()
{
    std::cout << "\n--- Testing clear() ---\n";
    TEditor ed;
    ed.addDigit(5);
    ed.addDigit(3);
    CHECK(ed.getString() != "0/1");
    ed.clear();
    CHECK(ed.getString() == "0/1");
    CHECK(ed.isZero() == true);
}

void testEditorAddDigit()
{
    std::cout << "\n--- Testing addDigit() ---\n";
    TEditor ed;
    ed.addDigit(5);
    CHECK(ed.getString() == "0/15");
    ed.addDigit(3);
    CHECK(ed.getString() == "0/153");
    ed.addDigit(10); // не цифра, игнорируется
    CHECK(ed.getString() == "0/153");
}

void testEditorAddZero()
{
    std::cout << "\n--- Testing addZero() ---\n";
    TEditor ed;
    ed.addZero();
    CHECK(ed.getString() == "0/10");
}

void testEditorToggleSign()
{
    std::cout << "\n--- Testing toggleSign() ---\n";
    TEditor ed;
    // изначально "0/1" без минуса
    ed.toggleSign();
    CHECK(ed.getString() == "-0/1");
    ed.toggleSign();
    CHECK(ed.getString() == "0/1");

    // проверка на пустой строке (setString устанавливает "0/1")
    TEditor ed2;
    ed2.setString("");
    ed2.toggleSign();
    CHECK(ed2.getString() == "-0/1"); // исправлено: раньше было "-"
}

void testEditorBackspace()
{
    std::cout << "\n--- Testing backspace() ---\n";
    TEditor ed;
    ed.addDigit(1);
    ed.addDigit(2);
    ed.addDigit(3);
    CHECK(ed.getString() == "0/1123"); // исправлено: ранее ожидалось "0/123"
    ed.backspace();
    CHECK(ed.getString() == "0/112"); // исправлено: ранее "0/12"
    ed.backspace();
    CHECK(ed.getString() == "0/11"); // исправлено: ранее "0/1"
    ed.backspace();
    CHECK(ed.getString() == "0/1"); // после третьего backspace остаётся "0/1"
}

void testEditorSetGet()
{
    std::cout << "\n--- Testing setString/getString ---\n";
    TEditor ed;
    ed.setString("5/8");
    CHECK(ed.getString() == "5/8");
    CHECK(ed.isZero() == false);
    ed.setString("");
    CHECK(ed.getString() == "0/1"); // пустая строка заменяется на ноль
}

void testEditorEdit()
{
    std::cout << "\n--- Testing edit() ---\n";
    TEditor ed;
    ed.edit(7);
    CHECK(ed.getString() == "0/17");
    ed.edit(-1);
    CHECK(ed.getString() == "-0/17");
    ed.edit(-2);
    CHECK(ed.getString() == "-0/1");
    ed.edit(-3);
    CHECK(ed.getString() == "0/1");
    ed.edit(100); // неизвестная команда
    CHECK(ed.getString() == "0/1");
}

// ------------------------------------------------------------------
int main()
{
    std::cout << "Starting TEditor tests...\n";

    testEditorConstructors();
    testEditorClear();
    testEditorAddDigit();
    testEditorAddZero();
    testEditorToggleSign();
    testEditorBackspace();
    testEditorSetGet();
    testEditorEdit();

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
