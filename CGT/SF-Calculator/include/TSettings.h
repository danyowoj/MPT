#ifndef TSETTINGS_H
#define TSETTINGS_H

#include <string>
#include "UFrac.h" // для DisplayFormat

enum class OperandSource
{
    Memory,
    Clipboard
};

struct ExtraSettings
{
    bool autoClear;
    int historySize;
};

class TSettings
{
private:
    DisplayFormat m_displayFormat;
    OperandSource m_operandSource;
    ExtraSettings m_extra;

public:
    TSettings();

    DisplayFormat displayFormat() const;
    OperandSource operandSource() const;
    const ExtraSettings& extra() const;

    void setDisplayFormat(DisplayFormat fmt);
    void setOperandSource(OperandSource src);
    void setExtra(const ExtraSettings& extra);

    void saveToFile(const std::string &filename) const;
    void loadFromFile(const std::string &filename);
};

#endif
