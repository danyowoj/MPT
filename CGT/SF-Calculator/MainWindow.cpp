#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include "SettingsDialog.h"
#include "HelpDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_settings(new TSettings())
    , m_history(new THistory(100))
    , m_ctrl(new TCtrl(m_settings, m_history))
    , m_historyModel(new QStringListModel(this))
    , m_clipboard()
{
    ui->setupUi(this);

    setWindowTitle(tr("Simple Fraction Calculator"));

    // Установка модели для списка истории
    ui->viewHistory->setModel(m_historyModel);

    // Связывание контроллера с настройками и историей
    m_ctrl->setSettings(m_settings);
    m_ctrl->setHistory(m_history);

    // Первоначальное обновление интерфейса
    updateDisplay();
    updateHistory();
    updateMemoryState();

    // Подключение кнопок
    setupButtons();

    // Подключение действий меню
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onActionSave);
    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::onActionLoad);
    connect(ui->actionPreferences, &QAction::triggered, this, &MainWindow::onActionPreferences);
    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::onActionHelp);
    connect(ui->actionAuthors, &QAction::triggered, this, &MainWindow::onActionAuthors);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_ctrl;
    delete m_history;
    delete m_settings;
}

void MainWindow::setupButtons()
{
    // Цифры
    connect(ui->b0, &QPushButton::clicked, this, [this]() { handleCommand(CMD_0); });
    connect(ui->b1, &QPushButton::clicked, this, [this]() { handleCommand(CMD_1); });
    connect(ui->b2, &QPushButton::clicked, this, [this]() { handleCommand(CMD_2); });
    connect(ui->b3, &QPushButton::clicked, this, [this]() { handleCommand(CMD_3); });
    connect(ui->b4, &QPushButton::clicked, this, [this]() { handleCommand(CMD_4); });
    connect(ui->b5, &QPushButton::clicked, this, [this]() { handleCommand(CMD_5); });
    connect(ui->b6, &QPushButton::clicked, this, [this]() { handleCommand(CMD_6); });
    connect(ui->b7, &QPushButton::clicked, this, [this]() { handleCommand(CMD_7); });
    connect(ui->b8, &QPushButton::clicked, this, [this]() { handleCommand(CMD_8); });
    connect(ui->b9, &QPushButton::clicked, this, [this]() { handleCommand(CMD_9); });

    // Операции
    connect(ui->bPlus, &QPushButton::clicked, this, [this]() { handleCommand(CMD_ADD); });
    connect(ui->bMinus, &QPushButton::clicked, this, [this]() { handleCommand(CMD_SUBTRACT); });
    connect(ui->bMultiply, &QPushButton::clicked, this, [this]() { handleCommand(CMD_MULTIPLY); });
    connect(ui->bDivide, &QPushButton::clicked, this, [this]() { handleCommand(CMD_DIVIDE); });

    // Функции
    connect(ui->bSquare, &QPushButton::clicked, this, [this]() { handleCommand(CMD_SQR); });
    connect(ui->bReciprocal, &QPushButton::clicked, this, [this]() { handleCommand(CMD_RECIPROCAL); });
    connect(ui->bToggleSign, &QPushButton::clicked, this, [this]() { handleCommand(CMD_NEGATE); });

    // Управление вводом
    connect(ui->bClear, &QPushButton::clicked, this, [this]() { handleCommand(CMD_CLEAR); });
    connect(ui->bClearEntry, &QPushButton::clicked, this, [this]() { handleCommand(CMD_CLEAR_ENTRY); });
    connect(ui->bBackSpace, &QPushButton::clicked, this, [this]() { handleCommand(CMD_BACKSPACE); });
    connect(ui->bWholeDevider, &QPushButton::clicked, this, [this]() { handleCommand(CMD_WHOLE_SEP); });

    // Равенство
    connect(ui->bEquals, &QPushButton::clicked, this, [this]() { handleCommand(CMD_EQUALS); });

    // Память
    connect(ui->bMemoryStore, &QPushButton::clicked, this, [this]() { handleCommand(CMD_MEM_STORE); });
    connect(ui->bMemoryRecall, &QPushButton::clicked, this, [this]() { handleCommand(CMD_MEM_RECALL); });
    connect(ui->bMemoryClear, &QPushButton::clicked, this, [this]() { handleCommand(CMD_MEM_CLEAR); });
    connect(ui->bMemoryAdd, &QPushButton::clicked, this, [this]() { handleCommand(CMD_MEM_ADD); });

    // Буфер обмена
    connect(ui->bCopy, &QPushButton::clicked, this, [this]() { handleCommand(CMD_COPY); });
    connect(ui->bPaste, &QPushButton::clicked, this, [this]() { handleCommand(CMD_PASTE); });
}

void MainWindow::handleCommand(int cmd)
{
    std::string memState;
    std::string result = m_ctrl->executeCommand(cmd, m_clipboard, memState);

    ui->lineDisplay->setText(QString::fromStdString(result));

    if (!memState.empty())
        ui->lineMemoryState->setText(QString::fromStdString(memState));
    else
        updateMemoryState();

    updateHistory();
}

void MainWindow::updateDisplay()
{
    ui->lineDisplay->setText(QString::fromStdString(m_ctrl->getDisplay()));
}

void MainWindow::updateHistory()
{
    QStringList entries;
    for (const auto &entry : m_history->getEntries())
        entries << QString::fromStdString(entry);
    m_historyModel->setStringList(entries);
    ui->viewHistory->scrollToBottom();
}

void MainWindow::updateMemoryState()
{
    ui->lineMemoryState->setText(QString::fromStdString(m_ctrl->getMemoryState()));
}

void MainWindow::onActionSave()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Settings"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    tr("Settings Files (*.ini);;All Files (*)"));
    if (!fileName.isEmpty())
    {
        m_settings->saveToFile(fileName.toStdString());
        QMessageBox::information(this, tr("Saved"), tr("Settings saved successfully."));
    }
}

void MainWindow::onActionLoad()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load Settings"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    tr("Settings Files (*.ini);;All Files (*)"));
    if (!fileName.isEmpty())
    {
        m_settings->loadFromFile(fileName.toStdString());
        m_history->setMaxSize(m_settings->extra().historySize);
        m_ctrl->applySettings();
        updateDisplay();
        QMessageBox::information(this, tr("Loaded"), tr("Settings loaded successfully."));
    }
}

void MainWindow::onActionPreferences()
{
    SettingsDialog dialog(*m_settings, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        *m_settings = dialog.getSettings();
        m_history->setMaxSize(m_settings->extra().historySize);
        m_ctrl->applySettings();
        updateDisplay();
    }
}

void MainWindow::onActionHelp()
{
    HelpDialog help(this);
    help.exec();
}

void MainWindow::onActionAuthors()
{
    QMessageBox::information(this, tr("Authors"),
                             tr("Developed by danyowoj & opepl"));
}
