#include <windows.h>

#include "../include/TProc.h"
#include <iostream>
#include <stdexcept>

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

#define CHECK_THROWS(expr, exception_type)                                                    \
    do                                                                                        \
    {                                                                                         \
        ++total_checks;                                                                       \
        try                                                                                   \
        {                                                                                     \
            expr;                                                                             \
            ++failed_checks;                                                                  \
            std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__                              \
                      << " – " << #expr << " did not throw " << #exception_type << std::endl; \
        }                                                                                     \
        catch (const exception_type &)                                                        \
        {                                                                                     \
            /* expected */                                                                    \
        }                                                                                     \
        catch (...)                                                                           \
        {                                                                                     \
            ++failed_checks;                                                                  \
            std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__                              \
                      << " – " << #expr << " threw a different exception" << std::endl;       \
        }                                                                                     \
    } while (false)

bool fracEqual(const TFrac &a, const TFrac &b)
{
    return a == b; // используем оператор ==, определённый в TFrac
}

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
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(25, 1))); // 5 * 5
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
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(13, 1))); // 9 + 4

    proc.repeatLastOperation();
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(17, 1))); // 13 + 4
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

    // Нажимаем + (устанавливаем операцию, первый операнд = текущее значение)
    proc.setOperation(Operation::Add);
    CHECK(proc.isOperationPending() == true);
    CHECK(proc.isSecondOperandReceived() == false);

    // Нажимаем = без ввода второго операнда
    proc.calculate();
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(10, 1))); // 5 + 5
}

void testFunctions()
{
    std::cout << "\n--- Test: функции Sqr, Reciprocal, Negate ---\n";
    TProc proc;

    proc.setOperand(TFrac(5, 1));
    proc.performFunction(Function::Square);
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(25, 1)));

    proc.setOperand(TFrac(2, 1));
    proc.performFunction(Function::Reciprocal);
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(1, 2)));

    proc.setOperand(TFrac(3, 4));
    proc.performFunction(Function::Negate);
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(-3, 4)));
}

void testExpression()
{
    std::cout << "\n--- Test: выражение (6/1 Sqr + 2/1 Sqr / 10/1 + 6/1 = 10/1) ---\n";
    TProc proc;

    proc.setOperand(TFrac(6, 1));
    proc.performFunction(Function::Square); // 36/1
    proc.setOperation(Operation::Add);
    proc.setOperand(TFrac(2, 1));
    proc.performFunction(Function::Square); // 4/1 (второй операнд)
    proc.setOperation(Operation::Divide);   // выполнит сложение 36+4=40, затем установит деление
    proc.setOperand(TFrac(10, 1));
    proc.setOperation(Operation::Add); // выполнит деление 40/10=4, затем установит сложение
    proc.setOperand(TFrac(6, 1));
    proc.calculate(); // 4 + 6 = 10
    CHECK(fracEqual(proc.getCurrentValue(), TFrac(10, 1)));
}

void testDivisionByZero()
{
    std::cout << "\n--- Test: деление на ноль ---\n";
    TProc proc;
    proc.setOperand(TFrac(1, 2));
    proc.setOperation(Operation::Divide);
    proc.setOperand(TFrac(0, 1));
    CHECK_THROWS(proc.calculate(), std::invalid_argument);
}

void testReciprocalOfZero()
{
    std::cout << "\n--- Test: обратная величина от нуля ---\n";
    TProc proc;
    proc.setOperand(TFrac(0, 1));
    CHECK_THROWS(proc.performFunction(Function::Reciprocal), std::invalid_argument);
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
    testFunctions();
    testExpression();
    testDivisionByZero();
    testReciprocalOfZero();

    std::cout << "\n========================================\n";
    std::cout << "Total checks: " << total_checks << "\n";
    std::cout << "Failed checks: " << failed_checks << "\n";
    if (failed_checks == 0)
        std::cout << "ALL TESTS PASSED!\n";
    else
        std::cout << "SOME TESTS FAILED!\n";
    return (failed_checks == 0) ? 0 : 1;
}
