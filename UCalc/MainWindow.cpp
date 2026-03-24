#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "TCtrl.h"
#include "CalcCommands.h"
#include <QActionGroup>
#include <QClipboard>
#include <QGuiApplication>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QFileDialog>
#include <QKeyEvent>

// Конструктор
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ctrl(nullptr)
    , currentMode(0)
    , currentBase(10)
    , lastOperation(None)
    , lastLeftOperand("")
    , lastRightOperand("")
    , useMemoryMode(true)
{
    ui->setupUi(this);

    ctrl = new TCtrl(currentMode, currentBase);

    historyModel = new QStringListModel(this);
    ui->listHistory->setModel(historyModel);

    setupModeActions();
    setupBaseActions();
    setupMemoryClipboardActions();   // новый вызов
    setupConnections();

    updateUIMode();
    updateBaseVisibility();
    updateDigitButtonsState();
    updateMemoryClipboardVisibility(); // применить начальное состояние
    ui->labelDisplay->setText("0");
    ui->labelMemState->setText("");
}

MainWindow::~MainWindow()
{
    delete ctrl;
    delete ui;
}

void MainWindow::setupConnections()
{
    // Цифры 0-9
    QList<QPushButton*> digitButtons = {
        ui->b0, ui->b1, ui->b2, ui->b3, ui->b4,
        ui->b5, ui->b6, ui->b7, ui->b8, ui->b9
    };
    for (auto btn : digitButtons) {
        connect(btn, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    }

    // Буквы A-F
    QList<QPushButton*> letterButtons = {
        ui->bA, ui->bB, ui->bC, ui->bD, ui->bE, ui->bF
    };
    for (auto btn : letterButtons) {
        connect(btn, &QPushButton::clicked, this, &MainWindow::onLetterClicked);
    }

    // Операции (+, -, *, /)
    connect(ui->bPlus, &QPushButton::clicked, this, &MainWindow::onOperationClicked);
    connect(ui->bMinus, &QPushButton::clicked, this, &MainWindow::onOperationClicked);
    connect(ui->bMultiply, &QPushButton::clicked, this, &MainWindow::onOperationClicked);
    connect(ui->bDivide, &QPushButton::clicked, this, &MainWindow::onOperationClicked);

    // Функции (1/x, x^2)
    connect(ui->bReciproccal, &QPushButton::clicked, this, &MainWindow::onFunctionClicked);
    connect(ui->bSquare, &QPushButton::clicked, this, &MainWindow::onFunctionClicked);

    // Память
    connect(ui->bMemStore, &QPushButton::clicked, this, &MainWindow::onMemoryClicked);
    connect(ui->bMemRecall, &QPushButton::clicked, this, &MainWindow::onMemoryClicked);
    connect(ui->bMemAdd, &QPushButton::clicked, this, &MainWindow::onMemoryClicked);
    connect(ui->bMemClear, &QPushButton::clicked, this, &MainWindow::onMemoryClicked);

    // Редактирование
    connect(ui->bClearEntry, &QPushButton::clicked, this, &MainWindow::onClearEntryClicked);
    connect(ui->bClear, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(ui->bBackspace, &QPushButton::clicked, this, &MainWindow::onBackspaceClicked);
    connect(ui->bSwitchSign, &QPushButton::clicked, this, &MainWindow::onSignClicked);
    connect(ui->bDot, &QPushButton::clicked, this, &MainWindow::onSeparatorClicked);

    // Равно
    connect(ui->bCalculate, &QPushButton::clicked, this, &MainWindow::onCalculateClicked);

    // Буфер обмена
    connect(ui->bCopy, &QPushButton::clicked, this, &MainWindow::onEditClicked);
    connect(ui->bPaste, &QPushButton::clicked, this, &MainWindow::onEditClicked);

    // Меню
    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::onHelp);
    connect(ui->actionAuthors, &QAction::triggered, this, &MainWindow::onAuthors);
    connect(ui->actionPreferences, &QAction::triggered, this, &MainWindow::onPreferences);
}

void MainWindow::setupModeActions()
{
    QActionGroup *modeGroup = new QActionGroup(this);
    modeGroup->addAction(ui->actionp_number);
    modeGroup->addAction(ui->actionfraction);
    modeGroup->addAction(ui->actionComplex_number);
    ui->actionp_number->setCheckable(true);
    ui->actionfraction->setCheckable(true);
    ui->actionComplex_number->setCheckable(true);
    ui->actionp_number->setChecked(true);

    ui->menuSettings->addSeparator();
    ui->menuSettings->addAction(ui->actionp_number);
    ui->menuSettings->addAction(ui->actionfraction);
    ui->menuSettings->addAction(ui->actionComplex_number);

    connect(ui->actionp_number, &QAction::triggered, this, &MainWindow::onModeChanged);
    connect(ui->actionfraction, &QAction::triggered, this, &MainWindow::onModeChanged);
    connect(ui->actionComplex_number, &QAction::triggered, this, &MainWindow::onModeChanged);
}

void MainWindow::setupBaseActions()
{
    QMenu *baseMenu = ui->menuSettings->addMenu("Base (p-digit)");
    QActionGroup *baseGroup = new QActionGroup(this);
    for (int i = 2; i <= 16; ++i) {
        QAction *act = new QAction(QString::number(i), this);
        act->setCheckable(true);
        baseGroup->addAction(act);
        baseMenu->addAction(act);
        connect(act, &QAction::triggered, this, &MainWindow::onBaseChanged);
        if (i == 10) act->setChecked(true);
    }
}

void MainWindow::setupMemoryClipboardActions()
{
    QMenu *modeMenu = ui->menuSettings->addMenu("Operand source");
    QActionGroup *group = new QActionGroup(this);

    QAction *memoryAction = new QAction("Memory", this);
    memoryAction->setCheckable(true);
    memoryAction->setChecked(useMemoryMode);
    group->addAction(memoryAction);
    modeMenu->addAction(memoryAction);
    connect(memoryAction, &QAction::triggered, [this]() {
        useMemoryMode = true;
        updateMemoryClipboardVisibility();
    });

    QAction *clipboardAction = new QAction("Clipboard", this);
    clipboardAction->setCheckable(true);
    clipboardAction->setChecked(!useMemoryMode);
    group->addAction(clipboardAction);
    modeMenu->addAction(clipboardAction);
    connect(clipboardAction, &QAction::triggered, [this]() {
        useMemoryMode = false;
        updateMemoryClipboardVisibility();
    });
}

void MainWindow::updateMemoryClipboardVisibility()
{
    // Группа памяти
    ui->bMemStore->setVisible(useMemoryMode);
    ui->bMemRecall->setVisible(useMemoryMode);
    ui->bMemAdd->setVisible(useMemoryMode);
    ui->bMemClear->setVisible(useMemoryMode);
    ui->labelMemState->setVisible(useMemoryMode);

    // Группа буфера обмена
    ui->bCopy->setVisible(!useMemoryMode);
    ui->bPaste->setVisible(!useMemoryMode);
}

void MainWindow::updateUIMode()
{
    bool isPNumber = (currentMode == 0);
    bool isFrac = (currentMode == 1);
    bool isComplex = (currentMode == 2);

    // Кнопка точки (bDot) всегда видима и меняет текст в зависимости от режима
    if (isPNumber) {
        ui->bDot->setText(".");
    } else if (isFrac) {
        ui->bDot->setText("\\");
    } else if (isComplex) {
        ui->bDot->setText("i*");
    }
    ui->bDot->setVisible(true);

    // Буквы A-F видны только в p-ичном режиме
    for (auto btn : {ui->bA, ui->bB, ui->bC, ui->bD, ui->bE, ui->bF}) {
        btn->setVisible(isPNumber);
    }
    updateBaseVisibility();
}

void MainWindow::updateBaseVisibility()
{
    if (currentMode != 0) return;
    bool showLetters = (currentBase >= 11);
    for (auto btn : {ui->bA, ui->bB, ui->bC, ui->bD, ui->bE, ui->bF}) {
        btn->setVisible(showLetters);
    }
}

// Новая функция: обновление доступности кнопок цифр и букв в зависимости от основания
void MainWindow::updateDigitButtonsState()
{
    // Включаем все кнопки цифр
    for (int i = 0; i <= 9; ++i) {
        QPushButton *btn = findChild<QPushButton*>(QString("b%1").arg(i));
        if (btn) btn->setEnabled(true);
    }
    for (auto btn : {ui->bA, ui->bB, ui->bC, ui->bD, ui->bE, ui->bF}) {
        if (btn->isVisible()) btn->setEnabled(true);
    }

    if (currentMode == 0) {
        for (int i = 0; i <= 9; ++i) {
            if (i >= currentBase) {
                QPushButton *btn = findChild<QPushButton*>(QString("b%1").arg(i));
                if (btn) btn->setEnabled(false);
            }
        }
        QMap<QString, int> letterMap = {{"A",10},{"B",11},{"C",12},{"D",13},{"E",14},{"F",15}};
        for (auto btn : {ui->bA, ui->bB, ui->bC, ui->bD, ui->bE, ui->bF}) {
            if (btn->isVisible()) {
                int value = letterMap[btn->text()];
                btn->setEnabled(value < currentBase);
            }
        }
    }
}

void MainWindow::updateDisplay(const QString &text)
{
    ui->labelDisplay->setText(text);
}

void MainWindow::updateMemIndicator(const QString &state)
{
    ui->labelMemState->setText(state);
}

void MainWindow::addHistoryEntry(const QString &entry)
{
    historyList.append(entry);
    historyModel->setStringList(historyList);
    ui->listHistory->scrollToBottom();
}

void MainWindow::onDigitClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    int digit = btn->text().toInt();
    unsigned cmd = cZero + static_cast<unsigned>(digit);
    std::string clipboard, memState;
    QString result = QString::fromStdString(ctrl->executeCommand(cmd, clipboard, memState));
    updateDisplay(result);
    updateMemIndicator(QString::fromStdString(memState));
}

void MainWindow::onLetterClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    QString letter = btn->text();
    int value = 0;
    if (letter == "A") value = 10;
    else if (letter == "B") value = 11;
    else if (letter == "C") value = 12;
    else if (letter == "D") value = 13;
    else if (letter == "E") value = 14;
    else if (letter == "F") value = 15;
    unsigned cmd = cZero + static_cast<unsigned>(value);
    std::string clipboard, memState;
    QString result = QString::fromStdString(ctrl->executeCommand(cmd, clipboard, memState));
    updateDisplay(result);
    updateMemIndicator(QString::fromStdString(memState));
}

void MainWindow::onOperationClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    unsigned cmd = 0;
    TOprtn op = None;
    if (btn == ui->bPlus) {
        cmd = cAdd;
        op = Add;
    } else if (btn == ui->bMinus) {
        cmd = cSub;
        op = Sub;
    } else if (btn == ui->bMultiply) {
        cmd = cMul;
        op = Mul;
    } else if (btn == ui->bDivide) {
        cmd = cDiv;
        op = Dvd;
    } else {
        return;
    }

    // Сохраняем левый операнд и операцию для повторного вычисления
    lastLeftOperand = ui->labelDisplay->text();
    lastOperation = op;
    lastRightOperand.clear();   // правый операнд будет введён позже

    // Отправляем команду операции в контроллер (сбрасывает редактор и устанавливает операцию)
    std::string clipboard, memState;
    QString result = QString::fromStdString(ctrl->executeCommand(cmd, clipboard, memState));
    updateDisplay(result);
    updateMemIndicator(QString::fromStdString(memState));
}

void MainWindow::onFunctionClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    unsigned cmd = (btn == ui->bReciproccal) ? cRev : cSqr;
    std::string clipboard, memState;
    QString result = QString::fromStdString(ctrl->executeCommand(cmd, clipboard, memState));
    updateDisplay(result);
    updateMemIndicator(QString::fromStdString(memState));
}

void MainWindow::onMemoryClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    unsigned cmd = 0;
    if (btn == ui->bMemStore) cmd = cMemStore;
    else if (btn == ui->bMemRecall) cmd = cMemRestore;
    else if (btn == ui->bMemAdd) cmd = cMemAdd;
    else if (btn == ui->bMemClear) cmd = cMemClear;
    std::string clipboard, memState;
    QString result = QString::fromStdString(ctrl->executeCommand(cmd, clipboard, memState));
    updateDisplay(result);
    updateMemIndicator(QString::fromStdString(memState));
}

void MainWindow::onEditClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    if (btn == ui->bCopy) {
        QGuiApplication::clipboard()->setText(ui->labelDisplay->text());
    } else if (btn == ui->bPaste) {
        QString text = QGuiApplication::clipboard()->text();
        std::string clipboard = text.toStdString();
        std::string memState;
        QString result = QString::fromStdString(ctrl->executeCommand(cPaste, clipboard, memState));
        updateDisplay(result);
        updateMemIndicator(QString::fromStdString(memState));
    }
}

void MainWindow::onCalculateClicked()
{
    if (lastOperation == None) return;

    if (lastRightOperand.isEmpty()) {
        lastRightOperand = ui->labelDisplay->text();
    }

    std::string leftStr = lastLeftOperand.toStdString();
    std::string rightStr = lastRightOperand.toStdString();
    std::string resultStr = ctrl->repeatOperation(leftStr, rightStr, lastOperation);
    QString result = QString::fromStdString(resultStr);
    updateDisplay(result);

    QString opStr;
    switch (lastOperation) {
    case Add: opStr = " + "; break;
    case Sub: opStr = " - "; break;
    case Mul: opStr = " * "; break;
    case Dvd: opStr = " / "; break;
    default: opStr = " ? "; break;
    }
    QString historyEntry = lastLeftOperand + opStr + lastRightOperand + " = " + result;
    addHistoryEntry(historyEntry);

    lastLeftOperand = result;
    // lastRightOperand остаётся тем же
}

void MainWindow::onClearEntryClicked()
{
    // Очищаем только строку ввода (CE)
    unsigned cmd = cClear;
    std::string clipboard, memState;
    QString result = QString::fromStdString(ctrl->executeCommand(cmd, clipboard, memState));
    updateDisplay(result);
    updateMemIndicator(QString::fromStdString(memState));
    // При очистке ввода не сбрасываем lastOperation и lastRightOperand
}

void MainWindow::onClearClicked()
{
    delete ctrl;
    ctrl = new TCtrl(currentMode, currentBase);
    QGuiApplication::clipboard()->clear();
    lastOperation = None;
    lastLeftOperand.clear();
    lastRightOperand.clear();
    updateDisplay("0");
    updateMemIndicator("");
    updateDigitButtonsState();
}

void MainWindow::onBackspaceClicked()
{
    unsigned cmd = cBackspace;
    std::string clipboard, memState;
    QString result = QString::fromStdString(ctrl->executeCommand(cmd, clipboard, memState));
    updateDisplay(result);
    updateMemIndicator(QString::fromStdString(memState));
}

void MainWindow::onSignClicked()
{
    unsigned cmd = cSign;
    std::string clipboard, memState;
    QString result = QString::fromStdString(ctrl->executeCommand(cmd, clipboard, memState));
    updateDisplay(result);
    updateMemIndicator(QString::fromStdString(memState));
}

void MainWindow::onSeparatorClicked()
{
    unsigned cmd = cSeparator;
    std::string clipboard, memState;
    QString result = QString::fromStdString(ctrl->executeCommand(cmd, clipboard, memState));
    updateDisplay(result);
    updateMemIndicator(QString::fromStdString(memState));
}

void MainWindow::onModeChanged()
{
    int newMode = 0;
    if (ui->actionp_number->isChecked()) newMode = 0;
    else if (ui->actionfraction->isChecked()) newMode = 1;
    else if (ui->actionComplex_number->isChecked()) newMode = 2;

    if (newMode != currentMode) {
        currentMode = newMode;
        ctrl->changeMode(currentMode, currentBase);
        updateUIMode();
        updateDigitButtonsState();
        updateDisplay("0");
        updateMemIndicator("");
        lastOperation = None;
        lastLeftOperand.clear();
        lastRightOperand.clear();
    }
}

void MainWindow::onBaseChanged()
{
    QAction *act = qobject_cast<QAction*>(sender());
    if (!act) return;
    int newBase = act->text().toInt();
    if (newBase >= 2 && newBase <= 16 && newBase != currentBase) {
        currentBase = newBase;
        if (currentMode == 0) {
            ctrl->changeMode(0, currentBase);
            updateBaseVisibility();
            updateDigitButtonsState();
            updateDisplay("0");
        }
    }
}

void MainWindow::onPreferences()
{
    QMessageBox::information(this, "Настройки", "Настройки будут доступны в следующей версии.");
}

void MainWindow::onHelp()
{
    QString helpText =
        "<h2>Universal Calculator</h2>"
        "<p>This calculator supports three number modes:</p>"
        "<ul>"
        "<li><b>P‑adic numbers</b> (base 2–16) – use digits 0–9 and A–F for bases ≥11.</li>"
        "<li><b>Common fractions</b> – format <i>a/b</i> (use the \"\\\" button).</li>"
        "<li><b>Complex numbers</b> – format <i>a + i*b</i> (use the \"i*\" button).</li>"
        "</ul>"
        "<p><b>Basic operations</b>: +, –, *, /, = (Calculate).</p>"
        "<p><b>Functions</b>: 1/x (reciprocal), x^2 (square).</p>"
        "<p><b>Editing</b>: CE (clear entry), C (full reset), <-- (backspace), +/- (sign change).</p>"
        "<p><b>Memory / Clipboard</b>: choose in <i>Settings → Operand source</i>. "
        "Memory mode shows MS, MR, M+, MC buttons; Clipboard mode shows Copy, Paste buttons.</p>"
        "<p><b>History</b>: all calculations are stored in the list on the bottom. ";

    QMessageBox::about(this, "Help", helpText);
}

void MainWindow::onAuthors()
{
    QMessageBox::information(this, "Authors",
                             "danyowoj & opepl");
}
