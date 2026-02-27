#include <windows.h>

#include "../core/TProc.h"
#include "../core/CalcEnums.h"
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

bool fracEqual(const TFrac &a, const TFrac &b)
{
    return a.equals(b);
}

// --------------------------------------------------------------
void testSingleOperation()
{
    std::cout << "\n--- Test: одиночная операция (5/1 + 2/1 = 7/1) ---\n";
    TProc proc;

    proc.setOperand(TFrac(5, 1));
    proc.setOperation(Operation::Add);
    CHECK(proc.isOperationPending() == true);
    CHECK(proc.isSecondOperandReceived() == false);
    CHECK(proc.getPendingOperation() == Operation::Add);

    proc.setOperand(TFrac(2, 1));
    CHECK(proc.isOperationPending() == true);
    CHECK(proc.isSecondOperandReceived() == true);
    CHECK(proc.getPendingOperation() == Operation::Add);

    proc.calculate();
    CHECK(proc.isOperationPending() == false);
    CHECK(proc.isSecondOperandReceived() == false);
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(7, 1)));
}

void testSingleOperandOperation()
{
    std::cout << "\n--- Test: операция с одним операндом (5/1 * = 25/1) ---\n";
    TProc proc;

    proc.setOperand(TFrac(5, 1));
    proc.setOperation(Operation::Multiply);
    CHECK(proc.isOperationPending() == true);
    CHECK(proc.isSecondOperandReceived() == false);
    proc.calculate();
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(25, 1)));
    CHECK(proc.isOperationPending() == false);
}

void testRepeatOperation()
{
    std::cout << "\n--- Test: повтор операции (5/1 + 4/1 = 9/1 = 13/1 = 17/1) ---\n";
    TProc proc;

    proc.setOperand(TFrac(5, 1));
    proc.setOperation(Operation::Add);
    proc.setOperand(TFrac(4, 1));
    proc.calculate();
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(9, 1)));

    proc.repeatLastOperation();
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(13, 1)));

    proc.repeatLastOperation();
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(17, 1)));
}

void testOperationOnDisplayedValue()
{
    std::cout << "\n--- Test: операция над отображаемым значением (2/1 + 3/1 = 5/1, затем + = 10/1) ---\n";
    TProc proc;

    proc.setOperand(TFrac(2, 1));
    proc.setOperation(Operation::Add);
    proc.setOperand(TFrac(3, 1));
    proc.calculate();
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(5, 1)));

    // Нажимаем + (устанавливаем операцию с текущим значением как первый операнд)
    proc.setOperation(Operation::Add);
    CHECK(proc.isOperationPending() == true);
    CHECK(proc.isSecondOperandReceived() == false);
    // Нажимаем = без ввода второго операнда
    proc.calculate();
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(10, 1))); // 5 + 5
}

void testFunction()
{
    std::cout << "\n--- Test: функция Sqr (5/1 -> 25/1) ---\n";
    TProc proc;

    proc.setOperand(TFrac(5, 1));
    proc.performFunction(Function::Square);
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(25, 1)));
    CHECK(proc.isOperationPending() == false);
    CHECK(proc.isSecondOperandReceived() == false);
}

void testExpression()
{
    std::cout << "\n--- Test: выражение (6/1 Sqr + 2/1 Sqr / 10/1 + 6/1 = 10/1) ---\n";
    TProc proc;

    proc.setOperand(TFrac(6, 1));
    proc.performFunction(Function::Square);
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(36, 1)));

    proc.setOperation(Operation::Add);
    CHECK(proc.isOperationPending() == true);
    CHECK(proc.isSecondOperandReceived() == false);

    proc.setOperand(TFrac(2, 1));
    CHECK(proc.isOperationPending() == true);
    CHECK(proc.isSecondOperandReceived() == true);

    proc.performFunction(Function::Square); // применяем к 2/1 -> 4/1
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(4, 1)));
    CHECK(proc.isOperationPending() == true);
    CHECK(proc.isSecondOperandReceived() == true); // второй операнд обновлён

    proc.setOperation(Operation::Divide); // сначала выполнит накопленное сложение
    // после calculate внутри setOperation: 36+4 = 40, результат 40/1, затем устанавливается деление
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(40, 1))); // результат сложения
    CHECK(proc.isOperationPending() == true);               // теперь ожидается деление
    CHECK(proc.isSecondOperandReceived() == false);

    proc.setOperand(TFrac(10, 1));
    CHECK(proc.isOperationPending() == true);
    CHECK(proc.isSecondOperandReceived() == true);

    proc.setOperation(Operation::Add);                     // выполнит деление 40/10 = 4, затем установит сложение
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(4, 1))); // результат деления
    CHECK(proc.isOperationPending() == true);
    CHECK(proc.isSecondOperandReceived() == false);

    proc.setOperand(TFrac(6, 1));
    CHECK(proc.isOperationPending() == true);
    CHECK(proc.isSecondOperandReceived() == true);

    proc.calculate(); // 4 + 6 = 10
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(10, 1)));
    CHECK(proc.isOperationPending() == false);
}

// --------------------------------------------------------------
int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "Starting TProc tests...\n";

    testSingleOperation();
    testSingleOperandOperation();
    testRepeatOperation();
    testOperationOnDisplayedValue();
    testFunction();
    testExpression();

    std::cout << "\n========================================\n";
    std::cout << "Total checks: " << total_checks << "\n";
    std::cout << "Failed checks: " << failed_checks << "\n";
    if (failed_checks == 0)
        std::cout << "ALL TESTS PASSED!\n";
    else
        std::cout << "SOME TESTS FAILED!\n";
    return (failed_checks == 0) ? 0 : 1;
}
