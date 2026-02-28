#include <windows.h>

#include "../include/THistory.h"
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

void testBasic()
{
    std::cout << "\n--- THistory basic ---\n";
    THistory h;
    CHECK(h.size() == 0);
    CHECK(h.getLastEntry().empty());

    h.addEntry("5/1 + 2/1 = 7/1");
    CHECK(h.size() == 1);
    CHECK(h.getLastEntry() == "5/1 + 2/1 = 7/1");

    h.addEntry("3/4 * 2/1 = 3/2");
    CHECK(h.size() == 2);
    CHECK(h.getLastEntry() == "3/4 * 2/1 = 3/2");

    auto entries = h.getEntries();
    CHECK(entries.size() == 2);
    CHECK(entries[0] == "5/1 + 2/1 = 7/1");
    CHECK(entries[1] == "3/4 * 2/1 = 3/2");

    h.clear();
    CHECK(h.size() == 0);
}

void testMaxSize()
{
    std::cout << "\n--- THistory max size ---\n";
    THistory h(3);
    h.addEntry("1");
    h.addEntry("2");
    h.addEntry("3");
    CHECK(h.size() == 3);
    CHECK(h.getEntries()[0] == "1");
    CHECK(h.getEntries()[2] == "3");

    h.addEntry("4");
    CHECK(h.size() == 3);
    CHECK(h.getEntries()[0] == "2");
    CHECK(h.getEntries()[1] == "3");
    CHECK(h.getEntries()[2] == "4");

    h.setMaxSize(2);
    CHECK(h.size() == 2);
    CHECK(h.getEntries()[0] == "3");
    CHECK(h.getEntries()[1] == "4");
}

void testEmptyAdd()
{
    std::cout << "\n--- THistory empty add ---\n";
    THistory h;
    h.addEntry("");
    CHECK(h.size() == 0);
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "Starting THistory tests...\n";
    testBasic();
    testMaxSize();
    testEmptyAdd();

    std::cout << "\n========================================\n";
    std::cout << "Total checks: " << total_checks << "\n";
    std::cout << "Failed checks: " << failed_checks << "\n";
    if (failed_checks == 0)
        std::cout << "ALL TESTS PASSED!\n";
    else
        std::cout << "SOME TESTS FAILED!\n";
    return (failed_checks == 0) ? 0 : 1;
}
