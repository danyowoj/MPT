#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SettingsDialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QClipboard>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_ctrl(&m_settings, &m_history)
{
    ui->setupUi(this);

    ui->viewHistory->setModel(&m_historyModel);

    QList<QPushButton*> digitButtons = {
        ui->b0, ui->b1, ui->b2, ui->b3, ui->b4,
        ui->b5, ui->b6, ui->b7, ui->b8, ui->b9
    };
    for (auto btn : digitButtons) {
        connect(btn, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    }

    connect(ui->bPlus, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(ui->bMinus, &QPushButton::clicked, this, &MainWindow::onSubtractClicked);
    connect(ui->bMultiply, &QPushButton::clicked, this, &MainWindow::onMultiplyClicked);
    connect(ui->bDivide, &QPushButton::clicked, this, &MainWindow::onDivideClicked);
    connect(ui->bEquals, &QPushButton::clicked, this, &MainWindow::onEqualsClicked);

    connect(ui->bSquare, &QPushButton::clicked, this, &MainWindow::onSquareClicked);
    connect(ui->bReciprocal, &QPushButton::clicked, this, &MainWindow::onReciprocalClicked);
    connect(ui->bToggleSign, &QPushButton::clicked, this, &MainWindow::onToggleSignClicked);

    connect(ui->bBackSpace, &QPushButton::clicked, this, &MainWindow::onBackspaceClicked);
    connect(ui->bClear, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(ui->bClearEntry, &QPushButton::clicked, this, &MainWindow::onClearEntryClicked);
    connect(ui->bWholeDevider, &QPushButton::clicked, this, &MainWindow::onWholeSeparatorClicked);

    connect(ui->bMemoryStore, &QPushButton::clicked, this, &MainWindow::onMemoryStoreClicked);
    connect(ui->bMemoryRecall, &QPushButton::clicked, this, &MainWindow::onMemoryRecallClicked);
    connect(ui->bMemoryAdd, &QPushButton::clicked, this, &MainWindow::onMemoryAddClicked);
    connect(ui->bMemoryClear, &QPushButton::clicked, this, &MainWindow::onMemoryClearClicked);

    connect(ui->bCopy, &QPushButton::clicked, this, &MainWindow::onCopyClicked);
    connect(ui->bPaste, &QPushButton::clicked, this, &MainWindow::onPasteClicked);

    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onActionSave);
    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::onActionLoad);
    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::onActionHelp);
    connect(ui->actionAuthors, &QAction::triggered, this, &MainWindow::onActionAuthors);

    QAction *settingsAction = new QAction(tr("Configure..."), this);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::onSettingsClicked);
    ui->menuSettings->addAction(settingsAction);

    updateDisplay();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendCommand(int cmd)
{
    QString clip = QGuiApplication::clipboard()->text();
    std::string clipboard = clip.toStdString();
    std::string memState; // больше не используем для обновления
    std::string result = m_ctrl.executeCommand(cmd, clipboard, memState);
    QGuiApplication::clipboard()->setText(QString::fromStdString(clipboard));

    ui->lineDisplay->setText(QString::fromStdString(result));
    ui->lineMemoryState->setText(QString("Memory: %1").arg(QString::fromStdString(m_ctrl.getMemoryState())));

    auto entries = m_history.getEntries();
    QStringList list;
    for (const auto& e : entries) {
        list << QString::fromStdString(e);
    }
    m_historyModel.setStringList(list);
}

void MainWindow::onDigitClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    int digit = btn->text().toInt();
    sendCommand(digit);
}

void MainWindow::onAddClicked() { sendCommand(CMD_ADD); }
void MainWindow::onSubtractClicked() { sendCommand(CMD_SUBTRACT); }
void MainWindow::onMultiplyClicked() { sendCommand(CMD_MULTIPLY); }
void MainWindow::onDivideClicked() { sendCommand(CMD_DIVIDE); }
void MainWindow::onEqualsClicked() { sendCommand(CMD_EQUALS); }

void MainWindow::onSquareClicked() { sendCommand(CMD_SQR); }
void MainWindow::onReciprocalClicked() { sendCommand(CMD_RECIPROCAL); }
void MainWindow::onToggleSignClicked() { sendCommand(CMD_TOGGLE_SIGN); }

void MainWindow::onBackspaceClicked() { sendCommand(CMD_BACKSPACE); }
void MainWindow::onClearClicked() { sendCommand(CMD_CLEAR); }
void MainWindow::onClearEntryClicked() { sendCommand(CMD_CLEAR_ENTRY); }
void MainWindow::onWholeSeparatorClicked() { sendCommand(CMD_WHOLE_SEP); }

void MainWindow::onMemoryStoreClicked() { sendCommand(CMD_MEM_STORE); }
void MainWindow::onMemoryRecallClicked() { sendCommand(CMD_MEM_RECALL); }
void MainWindow::onMemoryAddClicked() { sendCommand(CMD_MEM_ADD); }
void MainWindow::onMemoryClearClicked() { sendCommand(CMD_MEM_CLEAR); }

void MainWindow::onCopyClicked() { sendCommand(CMD_COPY); }
void MainWindow::onPasteClicked() { sendCommand(CMD_PASTE); }

void MainWindow::onActionSave()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Settings"), "", tr("Config Files (*.cfg)"));
    if (!fileName.isEmpty()) {
        m_settings.saveToFile(fileName.toStdString());
    }
}

void MainWindow::onActionLoad()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Settings"), "", tr("Config Files (*.cfg)"));
    if (!fileName.isEmpty()) {
        m_settings.loadFromFile(fileName.toStdString());
        updateDisplay();
    }
}

void MainWindow::onActionHelp()
{
    QMessageBox::information(this, tr("Help"), tr(
                                                   "Simple Fraction Calculator\n\n"
                                                   "Enter numbers using digit buttons.\n"
                                                   "Use '|' for whole part separator.\n"
                                                   "Operations: +, -, *, /, x^2, 1/x, +/-.\n"
                                                   "Memory: MS (store), MR (recall), MC (clear), M+ (add).\n"
                                                   "Copy/Paste via clipboard buttons.\n"
                                                   "History is shown on the left."
                                                   ));
}

void MainWindow::onActionAuthors()
{
    QMessageBox::information(this, tr("Authors"), tr("Developed by danyowoj & opepl."));
}

void MainWindow::onSettingsClicked()
{
    SettingsDialog dialog(m_settings, this);
    if (dialog.exec() == QDialog::Accepted) {
        updateDisplay();
    }
}

void MainWindow::updateDisplay()
{
    std::string display = m_ctrl.getDisplay();
    ui->lineDisplay->setText(QString::fromStdString(display));
    ui->lineMemoryState->setText(QString("Memory: %1").arg(QString::fromStdString(m_ctrl.getMemoryState())));
}
