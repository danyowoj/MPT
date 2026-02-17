#include "AboutDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("О программе");
    resize(400, 600); // увеличен размер для размещения справки

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Заголовок
    QLabel *titleLabel = new QLabel("<h2>Конвертор p1-p2</h2>");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // Описание программы
    QLabel *descLabel = new QLabel(
        "<p><b>Назначение:</b> Преобразование действительных чисел "
        "из системы счисления с основанием p1 в систему с основанием p2.</p>"
        );
    descLabel->setWordWrap(true);
    layout->addWidget(descLabel);

    // Справка по использованию
    QLabel *helpLabel = new QLabel(
        "<p><b>Использование:</b></p>"
        "<ul>"
        "<li>Выберите основание p1 (исходное число) и p2 (результат) с помощью слайдеров или счётчиков.</li>"
        "<li>Введите число, используя кнопки с цифрами или клавиатуру (0-9, A-F).</li>"
        "<li>Нажмите <b>Exec</b> или клавишу <b>Enter</b> для преобразования.</li>"
        "<li>Кнопка <b>.</b> добавляет десятичный разделитель.</li>"
        "<li>Кнопка <b>BS</b> удаляет последний символ (Backspace).</li>"
        "<li>Кнопка <b>CE</b> очищает поле ввода.</li>"
        "<li>Меню <b>История</b> показывает все выполненные преобразования.</li>"
        "</ul>"
        "<p><i>Примечание:</i> недопустимые для выбранного основания цифры автоматически блокируются.</p>"
        );
    helpLabel->setWordWrap(true);
    layout->addWidget(helpLabel);

    // Разработчики
    QLabel *devLabel = new QLabel(
        "<p><b>Разработчики:</b> danyowoj & opepl</p>"
        );
    devLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(devLabel);

    // Кнопка OK
    QPushButton *okBtn = new QPushButton("OK");
    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(okBtn, 0, Qt::AlignCenter);
}
