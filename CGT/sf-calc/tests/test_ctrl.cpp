#include <windows.h>

#include "../include/UCtrl.h"
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

#define CHECK_EQUAL_STR(a, b) CHECK((a) == (b))

void testDisplayFormat()
{
    std::cout << "\n--- TCtrl display format ---\n";
    TSettings settings;
    settings.setDisplayFormat(DisplayFormat::Decimal);
    THistory history;
    TCtrl ctrl(&settings, &history);

    std::string clip = "1/2";
    std::string mem;

    ctrl.executeCommand(CMD_PASTE, clip, mem);
    // Дробь 1/2 представима десятичной, поэтому должно отобразиться как 0.5
    std::string disp = ctrl.getDisplay();
    CHECK_EQUAL_STR(disp, "0.5");

    // Проверим, что при Fraction отображается дробь
    settings.setDisplayFormat(DisplayFormat::Fraction);
    disp = ctrl.getDisplay();
    CHECK_EQUAL_STR(disp, "1/2");
}

void testHistoryAddition()
{
    std::cout << "\n--- TCtrl history addition ---\n";
    THistory history;
    TCtrl ctrl(nullptr, &history);

    std::string clip = "3/4";
    std::string mem;

    ctrl.executeCommand(CMD_PASTE, clip, mem);
    ctrl.executeCommand(CMD_SQR, clip, mem); // (3/4)^2 = 9/16
    CHECK(history.size() == 1);
    CHECK_EQUAL_STR(history.getLastEntry(), "3/4 sqr = 9/16");

    ctrl.executeCommand(CMD_PASTE, clip = "1/2", mem);
    ctrl.executeCommand(CMD_ADD, clip, mem);
    ctrl.executeCommand(CMD_PASTE, clip = "1/3", mem);
    ctrl.executeCommand(CMD_EQUALS, clip, mem); // 1/2 + 1/3 = 5/6
    CHECK(history.size() == 2);
    CHECK_EQUAL_STR(history.getLastEntry(), "= 5/6");
}

void testOperandSourceMemory()
{
    std::cout << "\n--- TCtrl operand source memory ---\n";
    TSettings settings;
    settings.setOperandSource(OperandSource::Memory);
    TCtrl ctrl(&settings, nullptr);
    std::string clip, mem;

    // Сохраняем 5/1 в память
    ctrl.executeCommand(CMD_PASTE, clip = "5/1", mem);
    ctrl.executeCommand(CMD_MEM_STORE, clip, mem);

    // Вводим 2/1 как первый операнд
    ctrl.executeCommand(CMD_PASTE, clip = "2/1", mem);
    // Нажимаем + (должно автоматически подставить из памяти)
    ctrl.executeCommand(CMD_ADD, clip, mem);
    // Проверим, что текущее отображение стало 5/1 (число из памяти)
    CHECK_EQUAL_STR(ctrl.getDisplay(), "5/1");
    // Нажимаем =
    ctrl.executeCommand(CMD_EQUALS, clip, mem);
    CHECK_EQUAL_STR(ctrl.getDisplay(), "7/1");
}

// ------------------------------------------------------------------
int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "Starting TCtrl integration tests...\n";
    testDisplayFormat();
    testHistoryAddition();
    testOperandSourceMemory();

    std::cout << "\n========================================\n";
    std::cout << "Total checks: " << total_checks << "\n";
    std::cout << "Failed checks: " << failed_checks << "\n";
    if (failed_checks == 0)
        std::cout << "ALL TESTS PASSED!\n";
    else
        std::cout << "SOME TESTS FAILED!\n";
    return (failed_checks == 0) ? 0 : 1;
}
