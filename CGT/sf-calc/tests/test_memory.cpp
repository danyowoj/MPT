#include <windows.h>

#include "../include/UMemory.h"
#include "../include/UFrac.h"
#include <iostream>
#include <string>

// Макросы для тестирования
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

// Временно для тестов:
TFrac operator+(const TFrac &a, const TFrac &b)
{
    return a.add(b);
}

// ------------------------------------------------------------------
void testMemoryConstructor()
{
    std::cout << "\n--- Testing TMemory constructor ---\n";
    TMemory<TFrac> mem;
    CHECK(mem.getState() == MemoryState::Off);
    CHECK(mem.stateString() == "_Off");
    TFrac zero; // 0/1
    CHECK(mem.getNumber().equals(zero));
}

void testMemoryStoreAndRecall()
{
    std::cout << "\n--- Testing store() and recall() ---\n";
    TMemory<TFrac> mem;
    TFrac f(3, 4);
    mem.store(f);
    CHECK(mem.getState() == MemoryState::On);
    CHECK(mem.stateString() == "_On");
    TFrac recalled = mem.recall();
    CHECK(recalled.equals(f));
    // Проверим, что recall возвращает копию, а не ссылку (меняем копию, оригинал не должен измениться)
    recalled = TFrac(1, 2);
    CHECK(mem.recall().equals(f)); // в памяти по-прежнему 3/4
}

void testMemoryAdd()
{
    std::cout << "\n--- Testing add() ---\n";
    TMemory<TFrac> mem;
    TFrac a(1, 2);
    mem.store(a);
    TFrac b(1, 3);
    mem.add(b); // должно стать 1/2 + 1/3 = 5/6
    TFrac expected(5, 6);
    CHECK(mem.recall().equals(expected));
    CHECK(mem.getState() == MemoryState::On);
}

void testMemoryClear()
{
    std::cout << "\n--- Testing clear() ---\n";
    TMemory<TFrac> mem;
    mem.store(TFrac(2, 5));
    mem.clear();
    CHECK(mem.getState() == MemoryState::Off);
    CHECK(mem.stateString() == "_Off");
    TFrac zero;
    CHECK(mem.getNumber().equals(zero));
}

void testMemoryStateString()
{
    std::cout << "\n--- Testing stateString() ---\n";
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

    testMemoryConstructor();
    testMemoryStoreAndRecall();
    testMemoryAdd();
    testMemoryClear();
    testMemoryStateString();

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
