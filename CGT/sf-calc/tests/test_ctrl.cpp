#include <windows.h>

#include "../include/UCtrl.h"
#include <iostream>
#include <string>

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

bool fracEqual(const TFrac &a, const TFrac &b)
{
    return a.equals(b);
}

// Вспомогательная функция для сравнения строк (игнорируем возможные пробелы)
bool stringEqual(const std::string &a, const std::string &b)
{
    return a == b;
}

// ------------------------------------------------------------------
void testPasteAndOperation()
{
    std::cout << "\n--- Test: вставка из буфера и операция 1/2 + 1/3 = 5/6 ---\n";
    TCtrl ctrl;
    std::string clipboard = "1/2";
    std::string memState;

    // Вставляем 1/2
    std::string disp = ctrl.executeCommand(CMD_PASTE, clipboard, memState);
    CHECK(stringEqual(disp, "1/2"));
    CHECK(ctrl.getState() == TCtrlState::cEditing);

    // Устанавливаем операцию сложения
    disp = ctrl.executeCommand(CMD_ADD, clipboard, memState);
    CHECK(ctrl.getState() == TCtrlState::cOpChange);
    // Отображение не должно измениться
    CHECK(stringEqual(disp, "1/2"));

    // Меняем буфер на 1/3 и вставляем как второй операнд
    clipboard = "1/3";
    disp = ctrl.executeCommand(CMD_PASTE, clipboard, memState);
    CHECK(stringEqual(disp, "1/3"));
    CHECK(ctrl.getState() == TCtrlState::cEditing);

    // Вычисляем
    disp = ctrl.executeCommand(CMD_EQUALS, clipboard, memState);
    // Ожидаем 5/6
    CHECK(stringEqual(disp, "5/6"));
    CHECK(ctrl.getState() == TCtrlState::cExpDone);
}

void testMemoryWithPaste()
{
    TCtrl ctrl;
    std::string clipboard = "3/4";
    std::string memState;

    std::string disp = ctrl.executeCommand(CMD_PASTE, clipboard, memState);
    CHECK(stringEqual(disp, "3/4"));

    disp = ctrl.executeCommand(CMD_MEM_STORE, clipboard, memState);
    CHECK(memState == "_On");

    disp = ctrl.executeCommand(CMD_CLEAR, clipboard, memState);
    CHECK(stringEqual(disp, "0/1"));

    disp = ctrl.executeCommand(CMD_MEM_RECALL, clipboard, memState);
    CHECK(stringEqual(disp, "3/4")); // проверка, что память вернула 3/4

    clipboard = "1/4";
    disp = ctrl.executeCommand(CMD_PASTE, clipboard, memState);
    CHECK(stringEqual(disp, "1/4"));

    disp = ctrl.executeCommand(CMD_MEM_ADD, clipboard, memState);
    // Сразу после добавления проверим память через RECALL
    disp = ctrl.executeCommand(CMD_MEM_RECALL, clipboard, memState);
    CHECK(stringEqual(disp, "1/1")); // должно быть 1/1

    // Далее как раньше
    disp = ctrl.executeCommand(CMD_CLEAR, clipboard, memState);
    disp = ctrl.executeCommand(CMD_MEM_RECALL, clipboard, memState);
    CHECK(stringEqual(disp, "1/1"));

    disp = ctrl.executeCommand(CMD_MEM_CLEAR, clipboard, memState);
    CHECK(memState == "_Off");
}

void testClipboardCopy()
{
    std::cout << "\n--- Test: копирование в буфер обмена ---\n";
    TCtrl ctrl;
    std::string clipboard;
    std::string memState;

    // Устанавливаем число через вставку
    clipboard = "7/8";
    std::string disp = ctrl.executeCommand(CMD_PASTE, clipboard, memState);
    CHECK(stringEqual(disp, "7/8"));

    // Копируем в буфер
    clipboard = ""; // очистим перед копированием
    disp = ctrl.executeCommand(CMD_COPY, clipboard, memState);
    CHECK(stringEqual(clipboard, "7/8"));
}

void testClear()
{
    std::cout << "\n--- Test: очистка ---\n";
    TCtrl ctrl;
    std::string clipboard = "2/3";
    std::string memState;

    ctrl.executeCommand(CMD_PASTE, clipboard, memState);
    ctrl.executeCommand(CMD_MEM_STORE, clipboard, memState);
    ctrl.executeCommand(CMD_CLEAR, clipboard, memState);
    CHECK(stringEqual(ctrl.getDisplay(), "0/1"));
    CHECK(ctrl.getState() == TCtrlState::cStart);
    // Память не должна очиститься
    CHECK(memState == "_On");
}

// ------------------------------------------------------------------
int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "Starting TCtrl tests...\n";

    testPasteAndOperation();
    testMemoryWithPaste();
    testClipboardCopy();
    testClear();

    std::cout << "\n========================================\n";
    std::cout << "Total checks: " << total_checks << "\n";
    std::cout << "Failed checks: " << failed_checks << "\n";
    if (failed_checks == 0)
        std::cout << "ALL TESTS PASSED!\n";
    else
        std::cout << "SOME TESTS FAILED!\n";
    return (failed_checks == 0) ? 0 : 1;
}
