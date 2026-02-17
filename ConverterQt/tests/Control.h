#pragma once

#include "Editor.h"
#include "History.h"
#include <string>

class Control
{
public:
    enum class State
    {
        Editing,
        Converted
    };

    Control();

    // Управление основаниями

    int getPin() const;
    void setPin(int p);
    int getPout() const;
    void setPout(int p);

    // Текущее состояние
    State getState() const;

    std::string doCommand(int cmd);

    History his;

    Editor ed;
    int pin;
    int pout;
    State state;

    // Вычисление точности
    int accuracy() const;
};
