#include <windows.h>

#include "../include/UMemory.h"
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

// Для удобства используем оператор ==, определённый в TFrac
bool fracEqual(const TFrac &a, const TFrac &b)
{
    return a == b;
}

void testConstructor()
{
    std::cout << "\n--- TMemory constructor ---\n";
    TMemory<TFrac> mem;
    CHECK(mem.getState() == MemoryState::Off);
    CHECK(mem.stateString() == "_Off");
    CHECK(mem.getNumber() == TFrac());
}

void testStoreAndRecall()
{
    std::cout << "\n--- TMemory store/recall ---\n";
    TMemory<TFrac> mem;
    TFrac val(3, 4);
    mem.store(val);
    CHECK(mem.getState() == MemoryState::On);
    CHECK(mem.stateString() == "_On");
    CHECK(mem.recall() == val);
    // Изменение копии не влияет на память
    TFrac copy = mem.recall();
    copy = TFrac(1, 2);
    CHECK(mem.recall() == val);
}

void testAdd()
{
    std::cout << "\n--- TMemory add ---\n";
    TMemory<TFrac> mem;
    mem.store(TFrac(1, 2));
    mem.add(TFrac(1, 3)); // 1/2 + 1/3 = 5/6
    CHECK(mem.recall() == TFrac(5, 6));
    CHECK(mem.getState() == MemoryState::On);
}

void testClear()
{
    std::cout << "\n--- TMemory clear ---\n";
    TMemory<TFrac> mem;
    mem.store(TFrac(5, 1));
    mem.clear();
    CHECK(mem.getState() == MemoryState::Off);
    CHECK(mem.getNumber() == TFrac()); // 0/1
}

void testStateString()
{
    std::cout << "\n--- TMemory stateString ---\n";
    TMemory<TFrac> mem;
    CHECK(mem.stateString() == "_Off");
    mem.store(TFrac(1, 1));
    CHECK(mem.stateString() == "_On");
    mem.clear();
    CHECK(mem.stateString() == "_Off");
}

// ------------------------------------------------------------------
int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "Starting TMemory<TFrac> tests...\n";
    testConstructor();
    testStoreAndRecall();
    testAdd();
    testClear();
    testStateString();

    std::cout << "\n========================================\n";
    std::cout << "Total checks: " << total_checks << "\n";
    std::cout << "Failed checks: " << failed_checks << "\n";
    if (failed_checks == 0)
        std::cout << "ALL TESTS PASSED!\n";
    else
        std::cout << "SOME TESTS FAILED!\n";
    return (failed_checks == 0) ? 0 : 1;
}
