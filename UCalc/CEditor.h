#ifndef CEDITOR_H
#define CEDITOR_H

#include "TAEditor.h"

class CEditor : public TAEditor
{
private:
    bool hasSeparator; // введён разделитель "i*"
    bool isNegative;   // знак действительной части
    bool imNegative;   // знак мнимой части (хранится в строке)
    void updateFlags();

public:
    CEditor();
    CEditor(const CEditor &other);
    ~CEditor() override {}

    std::string addDigit(int digit) override;
    std::string addSign() override;
    std::string addSeparator() override;
    std::string backspace() override;
    std::string clear() override;

    bool isZero() const override;

    std::string getString() const override;
    void setString(const std::string &str) override;
};

#endif
