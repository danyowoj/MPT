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

bool fracEqual(const TFrac &a, const TFrac &b)
{
    return a == b;
}

// ------------------------------------------------------------------
void testInitialState()
{
    std::cout << "\n--- TCtrl initial state ---\n";
    TCtrl ctrl;
    std::string clip, mem;
    CHECK_EQUAL_STR(ctrl.getDisplay(), "0/1");
    CHECK(ctrl.getState() == TCtrlState::cStart);
}

void testDigitInput()
{
    std::cout << "\n--- TCtrl digit input ---\n";
    TCtrl ctrl;
    std::string clip, mem;
    ctrl.executeCommand(CMD_1, clip, mem);
    CHECK_EQUAL_STR(ctrl.getDisplay(), "0/11"); // редактор добавляет к "0/1"
    CHECK(ctrl.getState() == TCtrlState::cEditing);
    ctrl.executeCommand(CMD_2, clip, mem);
    CHECK_EQUAL_STR(ctrl.getDisplay(), "0/112");
}

void testClear()
{
    std::cout << "\n--- TCtrl clear ---\n";
    TCtrl ctrl;
    std::string clip, mem;
    ctrl.executeCommand(CMD_1, clip, mem);
    ctrl.executeCommand(CMD_CLEAR, clip, mem);
    CHECK_EQUAL_STR(ctrl.getDisplay(), "0/1");
    CHECK(ctrl.getState() == TCtrlState::cStart);
}

void testOperation()
{
    std::cout << "\n--- TCtrl operation 5/1 + 2/1 = 7/1 (using paste) ---\n";
    TCtrl ctrl;
    std::string clip = "5/1";
    std::string mem;

    // Вставляем 5/1
    ctrl.executeCommand(CMD_PASTE, clip, mem);
    CHECK_EQUAL_STR(ctrl.getDisplay(), "5/1");
    CHECK(ctrl.getState() == TCtrlState::cEditing);

    // Устанавливаем операцию +
    ctrl.executeCommand(CMD_ADD, clip, mem);
    CHECK(ctrl.getState() == TCtrlState::cOpChange);

    // Вставляем 2/1
    clip = "2/1";
    ctrl.executeCommand(CMD_PASTE, clip, mem);
    CHECK_EQUAL_STR(ctrl.getDisplay(), "2/1");
    CHECK(ctrl.getState() == TCtrlState::cEditing);

    // Вычисляем
    ctrl.executeCommand(CMD_EQUALS, clip, mem);
    CHECK_EQUAL_STR(ctrl.getDisplay(), "7/1");
    CHECK(ctrl.getState() == TCtrlState::cExpDone);
}

void testFunction()
{
    std::cout << "\n--- TCtrl function sqr(3/4) ---\n";
    TCtrl ctrl;
    std::string clip = "3/4";
    std::string mem;

    ctrl.executeCommand(CMD_PASTE, clip, mem);
    ctrl.executeCommand(CMD_SQR, clip, mem);
    CHECK_EQUAL_STR(ctrl.getDisplay(), "9/16");
    CHECK(ctrl.getState() == TCtrlState::cFunDone);
}

void testMemory()
{
    std::cout << "\n--- TCtrl memory ---\n";
    TCtrl ctrl;
    std::string clip = "2/3";
    std::string mem;

    ctrl.executeCommand(CMD_PASTE, clip, mem);
    ctrl.executeCommand(CMD_MEM_STORE, clip, mem);
    CHECK_EQUAL_STR(mem, "_On");

    ctrl.executeCommand(CMD_CLEAR, clip, mem);
    CHECK_EQUAL_STR(ctrl.getDisplay(), "0/1");

    ctrl.executeCommand(CMD_MEM_RECALL, clip, mem);
    CHECK_EQUAL_STR(ctrl.getDisplay(), "2/3");

    ctrl.executeCommand(CMD_MEM_ADD, clip, mem); // добавим текущее 2/3
    // теперь в памяти 2/3 + 2/3 = 4/3
    ctrl.executeCommand(CMD_CLEAR, clip, mem);
    ctrl.executeCommand(CMD_MEM_RECALL, clip, mem);
    CHECK_EQUAL_STR(ctrl.getDisplay(), "4/3");

    ctrl.executeCommand(CMD_MEM_CLEAR, clip, mem);
    CHECK_EQUAL_STR(mem, "_Off");
}

void testClipboard()
{
    std::cout << "\n--- TCtrl clipboard ---\n";
    TCtrl ctrl;
    std::string clip;
    std::string mem;

    ctrl.executeCommand(CMD_PASTE, clip, mem); // пустой буфер – ничего не делает
    CHECK_EQUAL_STR(ctrl.getDisplay(), "0/1");

    clip = "7/8";
    ctrl.executeCommand(CMD_PASTE, clip, mem);
    CHECK_EQUAL_STR(ctrl.getDisplay(), "7/8");

    ctrl.executeCommand(CMD_COPY, clip, mem);
    CHECK_EQUAL_STR(clip, "7/8");
}

void testErrorHandling()
{
    std::cout << "\n--- TCtrl error handling ---\n";
    TCtrl ctrl;
    std::string clip, mem;

    // Попытка взять обратную величину от нуля
    ctrl.executeCommand(CMD_PASTE, clip = "0/1", mem);
    ctrl.executeCommand(CMD_RECIPROCAL, clip, mem);
    CHECK(ctrl.getState() == TCtrlState::cError); // состояние ошибки
    // Можно также проверить, что отображение не изменилось или показывает что-то
}

// ------------------------------------------------------------------
int main()
{
    std::cout << "Starting TCtrl tests...\n";

    testInitialState();
    testDigitInput();
    testClear();
    testOperation();
    testFunction();
    testMemory();
    testClipboard();
    testErrorHandling();

    std::cout << "\n========================================\n";
    std::cout << "Total checks: " << total_checks << "\n";
    std::cout << "Failed checks: " << failed_checks << "\n";
    if (failed_checks == 0)
        std::cout << "ALL TESTS PASSED!\n";
    else
        std::cout << "SOME TESTS FAILED!\n";
    return (failed_checks == 0) ? 0 : 1;
}
