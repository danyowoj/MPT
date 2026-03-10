#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "include/TSettings.h"

class QComboBox;
class QCheckBox;
class QSpinBox;
class QDialogButtonBox;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(const TSettings &settings, QWidget *parent = nullptr);
    TSettings getSettings() const;

private:
    void setupUI();

    TSettings m_originalSettings;
    QComboBox *m_cmbDisplayFormat;
    QComboBox *m_cmbOperandSource;
    QSpinBox *m_spinHistorySize;
    QCheckBox *m_chkSoundEnabled;
    QDialogButtonBox *m_buttonBox;
};

#endif // SETTINGSDIALOG_H
