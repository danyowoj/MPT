#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>

// Подключаем заголовки всех классов
#include "Convert_10p.h"
#include "Convert_p10.h"
#include "Editor.h"
#include "History.h"
#include "Control.h"

using namespace std;

// Простая структура для сбора результатов тестов
struct TestResult
{
    int total = 0;
    int passed = 0;
    vector<string> errors;

    void addTest(const string &name, bool condition)
    {
        total++;
        if (condition)
        {
            passed++;
            cout << "[OK] " << name << endl;
        }
        else
        {
            errors.push_back(name);
            cout << "[FAIL] " << name << endl;
        }
    }

    void printSummary()
    {
        cout << "\n====================\n";
        cout << "Test results: " << passed << " / " << total << " passed.\n";
        if (!errors.empty())
        {
            cout << "Failed tests:\n";
            for (const auto &e : errors)
            {
                cout << "  - " << e << endl;
            }
        }
        else
        {
            cout << "All tests passed!" << endl;
        }
    }
};

void testConvert10p(TestResult &tr)
{
    cout << "\n--- Testing Convert_10p ---\n";

    // intToChar
    tr.addTest("Convert_10p::intToChar(0) == '0'", Convert_10p::intToChar(0) == '0');
    tr.addTest("Convert_10p::intToChar(5) == '5'", Convert_10p::intToChar(5) == '5');
    tr.addTest("Convert_10p::intToChar(9) == '9'", Convert_10p::intToChar(9) == '9');
    tr.addTest("Convert_10p::intToChar(10) == 'A'", Convert_10p::intToChar(10) == 'A');
    tr.addTest("Convert_10p::intToChar(15) == 'F'", Convert_10p::intToChar(15) == 'F');
    tr.addTest("Convert_10p::intToChar(16) == '?'", Convert_10p::intToChar(16) == '?');

    // intToP
    tr.addTest("Convert_10p::intToP(0,2) == \"0\"", Convert_10p::intToP(0, 2) == "0");
    tr.addTest("Convert_10p::intToP(10,2) == \"1010\"", Convert_10p::intToP(10, 2) == "1010");
    tr.addTest("Convert_10p::intToP(255,16) == \"FF\"", Convert_10p::intToP(255, 16) == "FF");
    tr.addTest("Convert_10p::intToP(161,16) == \"A1\"", Convert_10p::intToP(161, 16) == "A1");

    // fltToP
    tr.addTest("Convert_10p::fltToP(0.9375,2,4) == \"1111\"", Convert_10p::fltToP(0.9375, 2, 4) == "1111");
    tr.addTest("Convert_10p::fltToP(0.5,10,1) == \"5\"", Convert_10p::fltToP(0.5, 10, 1) == "5");
    // просто проверяем длину (не очень строго, но можно)
    tr.addTest("Convert_10p::fltToP(0.1,2,5) length == 5", Convert_10p::fltToP(0.1, 2, 5).size() == 5);

    // Do
    tr.addTest("Convert_10p::Do(0.0,10,0) == \"0\"", Convert_10p::Do(0.0, 10, 0) == "0");
    tr.addTest("Convert_10p::Do(17.875,16,3) == \"11.E00\"", Convert_10p::Do(17.875, 16, 3) == "11.E00");
    tr.addTest("Convert_10p::Do(-17.875,16,3) == \"-11.E00\"", Convert_10p::Do(-17.875, 16, 3) == "-11.E00");
    tr.addTest("Convert_10p::Do(255.0,16,0) == \"FF\"", Convert_10p::Do(255.0, 16, 0) == "FF");
    tr.addTest("Convert_10p::Do(3.1415,10,2) == \"3.14\"", Convert_10p::Do(3.1415, 10, 2) == "3.14");
}

void testConvertP10(TestResult &tr)
{
    cout << "\n--- Testing Convert_p10 ---\n";

    // charToNum
    tr.addTest("Convert_p10::charToNum('0') == 0", Convert_p10::charToNum('0') == 0);
    tr.addTest("Convert_p10::charToNum('5') == 5", Convert_p10::charToNum('5') == 5);
    tr.addTest("Convert_p10::charToNum('9') == 9", Convert_p10::charToNum('9') == 9);
    tr.addTest("Convert_p10::charToNum('A') == 10", Convert_p10::charToNum('A') == 10);
    tr.addTest("Convert_p10::charToNum('F') == 15", Convert_p10::charToNum('F') == 15);
    tr.addTest("Convert_p10::charToNum('a') == 10", Convert_p10::charToNum('a') == 10);
    tr.addTest("Convert_p10::charToNum('f') == 15", Convert_p10::charToNum('f') == 15);
    tr.addTest("Convert_p10::charToNum('G') == -1", Convert_p10::charToNum('G') == -1);

    // Do
    auto close = [](double a, double b)
    { return std::abs(a - b) < 1e-9; };
    tr.addTest("Convert_p10::Do(\"1010\",2) == 10.0", close(Convert_p10::Do("1010", 2), 10.0));
    tr.addTest("Convert_p10::Do(\"FF\",16) == 255.0", close(Convert_p10::Do("FF", 16), 255.0));
    tr.addTest("Convert_p10::Do(\"A1\",16) == 161.0", close(Convert_p10::Do("A1", 16), 161.0));
    tr.addTest("Convert_p10::Do(\"-11.E\",16) == -17.875", close(Convert_p10::Do("-11.E", 16), -17.875));
    tr.addTest("Convert_p10::Do(\"3.14\",10) == 3.14", close(Convert_p10::Do("3.14", 10), 3.14));
    tr.addTest("Convert_p10::Do(\"0.5\",10) == 0.5", close(Convert_p10::Do("0.5", 10), 0.5));
    tr.addTest("Convert_p10::Do(\"0\",10) == 0.0", close(Convert_p10::Do("0", 10), 0.0));
    tr.addTest("Convert_p10::Do(\"\",10) == 0.0", close(Convert_p10::Do("", 10), 0.0));
    tr.addTest("Convert_p10::Do(\"12G\",16) == 0.0", close(Convert_p10::Do("12G", 16), 0.0));
}

void testEditor(TestResult &tr)
{
    cout << "\n--- Testing Editor ---\n";

    Editor ed;

    tr.addTest("Editor initial number == \"0\"", ed.getNumber() == "0");
    tr.addTest("Editor initial accuracy == 0", ed.accuracy() == 0);

    ed.addDigit(1);
    tr.addTest("addDigit(1) -> \"1\"", ed.getNumber() == "1");
    ed.addDigit(2);
    tr.addTest("addDigit(2) -> \"12\"", ed.getNumber() == "12");
    ed.addDigit(10); // 'A'
    tr.addTest("addDigit(10) -> \"12A\"", ed.getNumber() == "12A");
    ed.addZero();
    tr.addTest("addZero() -> \"12A0\"", ed.getNumber() == "12A0");

    ed.addDelim();
    tr.addTest("addDelim() -> \"12A0.\"", ed.getNumber() == "12A0.");
    ed.addDelim(); // второй раз не добавляет
    tr.addTest("addDelim() twice -> still \"12A0.\"", ed.getNumber() == "12A0.");
    ed.addDigit(3);
    tr.addTest("addDigit(3) -> \"12A0.3\"", ed.getNumber() == "12A0.3");
    tr.addTest("accuracy now == 1", ed.accuracy() == 1);

    ed.backspace();
    tr.addTest("backspace() -> \"12A0.\"", ed.getNumber() == "12A0.");
    ed.backspace();
    tr.addTest("backspace() -> \"12A0\"", ed.getNumber() == "12A0");
    tr.addTest("accuracy now == 0", ed.accuracy() == 0);

    ed.clear();
    tr.addTest("clear() -> \"0\"", ed.getNumber() == "0");

    ed.addDelim();
    tr.addTest("addDelim() on empty -> \"0.\"", ed.getNumber() == "0.");

    ed.doCommand(5); // addDigit(5)
    tr.addTest("doCommand(5) -> \"0.5\"", ed.getNumber() == "0.5");
    ed.doCommand(16); // addDelim
    tr.addTest("doCommand(16) after \".5\" -> still \"0.5\" (no second dot)", ed.getNumber() == "0.5");
    ed.doCommand(17); // backspace
    tr.addTest("doCommand(17) -> \"0.\"", ed.getNumber() == "0.");
    ed.doCommand(18); // clear
    tr.addTest("doCommand(18) -> \"0\"", ed.getNumber() == "0");

    ed.clear();
    ed.addDigit(7);
    ed.addDelim();
    ed.addDigit(8);
    ed.addDigit(9);
    tr.addTest("accuracy after 7.89 == 2", ed.accuracy() == 2);
}

void testHistory(TestResult &tr)
{
    cout << "\n--- Testing History ---\n";

    History hist;
    tr.addTest("History initially empty", hist.count() == 0);

    hist.addRecord(16, 2, "A1.E", "10100001.111");
    tr.addTest("After addRecord, count == 1", hist.count() == 1);
    auto rec = hist.getRecord(0);
    tr.addTest("Record p1 == 16", rec.p1 == 16);
    tr.addTest("Record p2 == 2", rec.p2 == 2);
    tr.addTest("Record num1 == \"A1.E\"", rec.num1 == "A1.E");
    tr.addTest("Record num2 == \"10100001.111\"", rec.num2 == "10100001.111");

    string expected = "16 : A1.E -> 2 : 10100001.111\n";
    tr.addTest("Record toString() matches expected", rec.toString() == expected);

    hist.addRecord(8, 10, "123.4", "83.5");
    tr.addTest("Second add, count == 2", hist.count() == 2);
    rec = hist[1];
    tr.addTest("Operator[] works", rec.p1 == 8 && rec.p2 == 10 && rec.num1 == "123.4" && rec.num2 == "83.5");

    hist.clear();
    tr.addTest("After clear, count == 0", hist.count() == 0);
}

void testControl(TestResult &tr)
{
    cout << "\n--- Testing Control (integration) ---\n";

    Control ctl;
    tr.addTest("Initial pin == 10", ctl.getPin() == 10);
    tr.addTest("Initial pout == 16", ctl.getPout() == 16);
    tr.addTest("Initial state Editing", ctl.getState() == Control::State::Editing);

    // Ввод числа A.1 в 16-ричной
    ctl.setPin(16);
    ctl.setPout(2);
    ctl.doCommand(18); // clear
    ctl.doCommand(10); // A
    ctl.doCommand(16); // точка
    ctl.doCommand(1);  // 1

    string result = ctl.doCommand(19); // преобразование
    // A.1 (16) = 10.0625 (10) -> двоичная: 1010.0001
    tr.addTest("Conversion A.1 (16) -> binary with precision 4 yields 1010.0001", result == "1010.0001");

    tr.addTest("History count after conversion == 1", ctl.his.count() == 1);
    auto rec = ctl.his[0];
    tr.addTest("History record p1 == 16", rec.p1 == 16);
    tr.addTest("History record p2 == 2", rec.p2 == 2);
    tr.addTest("History record num1 == \"A.1\"", rec.num1 == "A.1");
    tr.addTest("History record num2 == \"1010.0001\"", rec.num2 == "1010.0001");

    // Изменение p2, состояние Converted, пересчёт
    ctl.setPout(16);
    result = ctl.doCommand(19);
    tr.addTest("Conversion back to hex yields A.1", result == "A.1");
}

int main()
{
    TestResult tr;

    testConvert10p(tr);
    testConvertP10(tr);
    testEditor(tr);
    testHistory(tr);
    testControl(tr);

    tr.printSummary();

    return tr.errors.empty() ? 0 : 1;
}
