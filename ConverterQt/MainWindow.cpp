#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QMenu>
#include "HistoryDialog.h"
#include "AboutDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_control(new Control())
{
    ui->setupUi(this);
    setWindowTitle("Конвертер систем счисления");

    // Запрещаем полям ввода получать фокус, чтобы все клавиши попадали в окно
    ui->lineEditInput->setFocusPolicy(Qt::NoFocus);
    ui->lineEditOutput->setFocusPolicy(Qt::NoFocus);

    // Инициализация отображения
    ui->lineEditInput->setText(QString::fromStdString(m_control->doCommand(18))); // очистка = "0"
    ui->lineEditOutput->setText("0");

    // Синхронизация начальных значений слайдеров и спинбоксов
    ui->sliderP1->setValue(m_control->getPin());
    ui->spinBoxP1->setValue(m_control->getPin());
    ui->sliderP2->setValue(m_control->getPout());
    ui->spinBoxP2->setValue(m_control->getPout());

    // Обработка Enter в полях ввода
    // connect(ui->lineEditInput, &QLineEdit::returnPressed, this, &MainWindow::on_bExec_clicked);
    // connect(ui->lineEditOutput, &QLineEdit::returnPressed, this, &MainWindow::on_bExec_clicked);

    // Настройка меню
    QAction *historyAction = new QAction("Показать историю", this);
    ui->menuHistory->addAction(historyAction);
    connect(historyAction, &QAction::triggered, this, &MainWindow::onHistoryTriggered);

    QAction *aboutAction = new QAction("О программе", this);
    ui->menuAbout->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAboutTriggered);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_control;
}

// -----------------------------
//* Слоты изменения основания p1
// -----------------------------
void MainWindow::on_sliderP1_valueChanged(int value)
{
    ui->spinBoxP1->setValue(value);
    updateP1(value);
}

void MainWindow::on_spinBoxP1_valueChanged(int arg1)
{
    ui->sliderP1->setValue(arg1);
    updateP1(arg1);
}

void MainWindow::updateP1(int value)
{
    m_control->setPin(value);
    // Сбрасываем результат, переходим в режим редактирования
    ui->lineEditInput->setText(QString::fromStdString(m_control->doCommand(18))); // clear
    ui->lineEditOutput->setText("0");
}

// -----------------------------
//* Слоты изменения основания p2
// -----------------------------
void MainWindow::on_sliderP2_valueChanged(int value)
{
    ui->spinBoxP2->setValue(value);
    updateP2(value);
}

void MainWindow::on_spinBoxP2_valueChanged(int arg1)
{
    ui->sliderP2->setValue(arg1);
    updateP2(arg1);
}

void MainWindow::updateP2(int value)
{
    m_control->setPout(value);
    // Если до этого было выполнено преобразование, пересчитываем результат
    if (m_control->getState() == Control::State::Converted)
        ui->lineEditOutput->setText(QString::fromStdString(m_control->doCommand(19)));
}

// -----------------------------
//* Вспомогательные методы для добавления отображения
// -----------------------------
void MainWindow::refreshInputDisplay()
{
    ui->lineEditInput->setText(QString::fromStdString(m_control->doCommand(-1))); // костыль для чтения Number
}

// -----------------------------
//* Обработка кнопок с цифрами
// -----------------------------
void MainWindow::on_b0_clicked() { executeDigit(0); }
void MainWindow::on_b1_clicked() { executeDigit(1); }
void MainWindow::on_b2_clicked() { executeDigit(2); }
void MainWindow::on_b3_clicked() { executeDigit(3); }
void MainWindow::on_b4_clicked() { executeDigit(4); }
void MainWindow::on_b5_clicked() { executeDigit(5); }
void MainWindow::on_b6_clicked() { executeDigit(6); }
void MainWindow::on_b7_clicked() { executeDigit(7); }
void MainWindow::on_b8_clicked() { executeDigit(8); }
void MainWindow::on_b9_clicked() { executeDigit(9); }
void MainWindow::on_bA_clicked() { executeDigit(10); }
void MainWindow::on_bB_clicked() { executeDigit(11); }
void MainWindow::on_bC_clicked() { executeDigit(12); }
void MainWindow::on_bD_clicked() { executeDigit(13); }
void MainWindow::on_bE_clicked() { executeDigit(14); }
void MainWindow::on_bF_clicked() { executeDigit(15); }

void MainWindow::executeDigit(int digit)
{
    int p1 = m_control->getPin();
    if (digit >= p1)
    {
        QMessageBox::warning(this, "Ошибка ввода",
                             QString("Цифра %1 недопустима в системе счисления с основанием %2.")
                                 .arg(QString::number(digit, 16).toUpper()).arg(p1));
        return;
    }

    std::string newNumber = m_control->doCommand(digit); // добавить цифру
    ui->lineEditInput->setText(QString::fromStdString(newNumber));
    ui->lineEditOutput->setText("0");
}

// -----------------------------
//* Остальные кнопки
// -----------------------------

void MainWindow::on_bDot_clicked()
{
    std::string newNumber = m_control->doCommand(16); // добавить разделитель
    ui->lineEditInput->setText(QString::fromStdString(newNumber));
    ui->lineEditOutput->setText("0");
}

void MainWindow::on_bMinus_clicked()
{
    std::string newNumber = m_control->doCommand(20); // смена знака
    ui->lineEditInput->setText(QString::fromStdString(newNumber));
    ui->lineEditOutput->setText("0");
}

void MainWindow::on_bBS_clicked()
{
    std::string newNumber = m_control->doCommand(17); // backspace
    ui->lineEditInput->setText(QString::fromStdString(newNumber));
    ui->lineEditOutput->setText("0");
}


void MainWindow::on_bCE_clicked()
{
    std::string newNumber = m_control->doCommand(18); // очистка
    ui->lineEditInput->setText(QString::fromStdString(newNumber));
    ui->lineEditOutput->setText("0");
}


void MainWindow::on_bExec_clicked()
{
    std::string result = m_control->doCommand(19); // конвертировать
    ui->lineEditOutput->setText(QString::fromStdString(result));
}

// -----------------------------
//* Обработка клавиатуры
// -----------------------------
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    Qt::KeyboardModifiers mod = event->modifiers();

    // Цифры 0-9
    if (key >= Qt::Key_0 && key <= Qt::Key_9)
    {
        int digit = key - Qt::Key_0;
        if (digit < m_control->getPin())
            executeDigit(digit);
        else
            showDigitError(digit);
        return;
    }

    // Буквы A-F (только верхний регистр)
    else if (key >= Qt::Key_A && key <= Qt::Key_F)
    {
        int digit = key - Qt::Key_A + 10;
        if (digit < m_control->getPin())
            executeDigit(digit);
        else
            showDigitError(digit);
        return;
    }

    // Точка
    else if (key == Qt::Key_Period || key == Qt::Key_Comma)
    {
        on_bDot_clicked();
        return;
    }

    // Смена знака
    else if (key == Qt::Key_Minus)
    {
        on_bMinus_clicked();
        return;
    }

    // Backspace
    else if (key == Qt::Key_Backspace)
    {
        on_bBS_clicked();
        return;
    }

    // Delete (функция clear)
    else if (key == Qt::Key_Delete)
    {
        on_bCE_clicked();
        return;
    }

    // Enter / Return
    else if (key == Qt::Key_Enter || key == Qt::Key_Return)
    {
        on_bExec_clicked();
        return;
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::showDigitError(int digit)
{
    QMessageBox::warning(this, "Ошибка ввода",
                         QString("Цифра %1 недопустима в системе счисления с основанием %2.")
                             .arg(QString::number(digit, 16).toUpper()).arg(m_control->getPin()));
}


// -----------------------------
//* Меню
// -----------------------------

void MainWindow::onHistoryTriggered()
{
    HistoryDialog dlg(&m_control->his, this);
    dlg.exec();
}

void MainWindow::onAboutTriggered()
{
    AboutDialog dlg(this);
    dlg.exec();
}
