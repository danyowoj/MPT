#include "TAEditor.h"

std::string TAEditor::executeCommand(unsigned cmd)
{
    if (cmd >= cZero && cmd <= cNine)
    {
        return addDigit(static_cast<int>(cmd));
    }
    switch (cmd)
    {
    case cSign:
        return addSign();
    case cSeparator:
        return addSeparator();
    case cBackspace:
        return backspace();
    case cClear:
        return clear();
    default:
        return fNumber;
    }
}
