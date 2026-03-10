#ifndef TSETTINGS_H
#define TSETTINGS_H

#include <string>
#include "UFrac.h" // для DisplayFormat

struct ExtraSettings
{
    int historySize;
};

class TSettings
{
private:
    DisplayFormat m_displayFormat;
    ExtraSettings m_extra;

public:
    TSettings();

    DisplayFormat displayFormat() const;
    const ExtraSettings& extra() const;

    void setDisplayFormat(DisplayFormat fmt);
    void setExtra(const ExtraSettings& extra);

    void saveToFile(const std::string &filename) const;
    void loadFromFile(const std::string &filename);
};

#endif
