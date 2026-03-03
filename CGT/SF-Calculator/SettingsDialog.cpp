#include "SettingsDialog.h"
#include <QRadioButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>

SettingsDialog::SettingsDialog(TSettings& settings, QWidget *parent)
    : QDialog(parent), m_settings(settings)
{
    setWindowTitle(tr("Settings"));

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Формат отображения
    layout->addWidget(new QLabel(tr("Display format:")));
    m_fracRadio = new QRadioButton(tr("Fraction"));
    m_decRadio = new QRadioButton(tr("Decimal"));
    QButtonGroup *fmtGroup = new QButtonGroup(this);
    fmtGroup->addButton(m_fracRadio);
    fmtGroup->addButton(m_decRadio);
    layout->addWidget(m_fracRadio);
    layout->addWidget(m_decRadio);

    if (m_settings.displayFormat() == DisplayFormat::Fraction)
        m_fracRadio->setChecked(true);
    else
        m_decRadio->setChecked(true);

    layout->addSpacing(10);

    // Источник операндов
    layout->addWidget(new QLabel(tr("Operand source:")));
    m_memRadio = new QRadioButton(tr("Memory"));
    m_clipRadio = new QRadioButton(tr("Clipboard"));
    QButtonGroup *srcGroup = new QButtonGroup(this);
    srcGroup->addButton(m_memRadio);
    srcGroup->addButton(m_clipRadio);
    layout->addWidget(m_memRadio);
    layout->addWidget(m_clipRadio);

    if (m_settings.operandSource() == OperandSource::Memory)
        m_memRadio->setChecked(true);
    else
        m_clipRadio->setChecked(true);

    layout->addSpacing(10);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::reject);
    layout->addWidget(m_buttonBox);
}

void SettingsDialog::accept()
{
    m_settings.setDisplayFormat(m_fracRadio->isChecked() ? DisplayFormat::Fraction : DisplayFormat::Decimal);
    m_settings.setOperandSource(m_memRadio->isChecked() ? OperandSource::Memory : OperandSource::Clipboard);
    QDialog::accept();
}
