#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "include/TSettings.h"

class QRadioButton;
class QDialogButtonBox;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(TSettings& settings, QWidget *parent = nullptr);

private slots:
    void accept() override;

private:
    TSettings& m_settings;
    QRadioButton *m_fracRadio;
    QRadioButton *m_decRadio;
    QRadioButton *m_memRadio;
    QRadioButton *m_clipRadio;
    QDialogButtonBox *m_buttonBox;
};

#endif
