#include "Control.h"
#include "Convert_10p.h"
#include "Convert_p10.h"
#include <cmath>

Control::Control() : pin(10), pout(16), state(State::Editing) {}

int Control::getPin() const { return pin; }

void Control::setPin(int p)
{
    if (p >= 2 && p <= 16)
        pin = p;
}

int Control::getPout() const { return pout; }

void Control::setPout(int p)
{
    if (p >= 2 && p <= 16)
        pout = p;
}

Control::State Control::getState() const { return state; }

std::string Control::doCommand(int cmd)
{
    if (cmd == 19)
    {
        std::string input = ed.getNumber();
        double decimal = Convert_p10::Do(input, pin);
        int precision = accuracy();
        std::string result = Convert_10p::Do(decimal, pout, precision);
        his.addRecord(pin, pout, input, result);
        state = State::Converted;
        return result;
    }
    else
    {
        ed.doCommand(cmd);
        state = State::Editing;
        return ed.getNumber();
    }
}

int Control::accuracy() const
{
    // Кол-во разрядов дробной части исходного числа
    int digits = ed.accuracy();

    if (digits == 0)
        return 0;

    // Отношение логарифмов оснований
    double ratio = std::log(static_cast<double>(pin)) / std::log(static_cast<double>(pout));
    double estimated = digits * ratio;

    // Округление до ближайшего целого
    return static_cast<int>(std::round(estimated));
}
