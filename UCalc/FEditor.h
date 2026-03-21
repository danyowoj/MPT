#ifndef FEDITOR_H
#define FEDITOR_H

#include "TAEditor.h"

class FEditor : public TAEditor
{
private:
    bool hasSeparator; // введён разделитель '/'
    bool isNegative;   // знак
    void updateFlags();

public:
    FEditor();
    FEditor(const FEditor &other);
    ~FEditor() override {}

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
