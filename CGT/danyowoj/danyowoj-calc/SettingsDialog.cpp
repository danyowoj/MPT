#include "SettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QGroupBox>

SettingsDialog::SettingsDialog(const TSettings &settings, QWidget *parent)
    : QDialog(parent)
    , m_originalSettings(settings)
{
    setupUI();

    // Устанавливаем текущие значения из настроек
    m_cmbDisplayFormat->setCurrentIndex(static_cast<int>(m_originalSettings.displayFormat()));
    m_spinHistorySize->setValue(m_originalSettings.extra().historySize);
}

void SettingsDialog::setupUI()
{
    setWindowTitle(tr("Settings"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // ---- Группа отображения ----
    QGroupBox *displayGroup = new QGroupBox(tr("Display"));
    QVBoxLayout *displayLayout = new QVBoxLayout(displayGroup);
    m_cmbDisplayFormat = new QComboBox();
    m_cmbDisplayFormat->addItem(tr("Fraction"));
    m_cmbDisplayFormat->addItem(tr("Decimal (if terminating)"));
    displayLayout->addWidget(new QLabel(tr("Display format:")));
    displayLayout->addWidget(m_cmbDisplayFormat);
    mainLayout->addWidget(displayGroup);

    // ---- Группа дополнительных настроек ----
    QGroupBox *extraGroup = new QGroupBox(tr("Extra"));
    QVBoxLayout *extraLayout = new QVBoxLayout(extraGroup);

    QHBoxLayout *historyLayout = new QHBoxLayout();
    historyLayout->addWidget(new QLabel(tr("History size:")));
    m_spinHistorySize = new QSpinBox();
    m_spinHistorySize->setRange(1, 1000);
    m_spinHistorySize->setValue(100);
    historyLayout->addWidget(m_spinHistorySize);
    extraLayout->addLayout(historyLayout);

    mainLayout->addWidget(extraGroup);

    // ---- Кнопки OK/Cancel ----
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(m_buttonBox);
}

TSettings SettingsDialog::getSettings() const
{
    TSettings result = m_originalSettings;
    result.setDisplayFormat(static_cast<DisplayFormat>(m_cmbDisplayFormat->currentIndex()));

    ExtraSettings extra;
    extra.historySize = m_spinHistorySize->value();
    result.setExtra(extra);

    return result;
}
