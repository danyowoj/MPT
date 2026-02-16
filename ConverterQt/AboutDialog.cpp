#include "AboutDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("О программе");
    resize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel(
        "<h4>Конвертор p1-p2</h4>"
        "<p>Преобразование действительных чисел из системы с основанием p1 в систему с основанием p2.</p>"
        "<p>Разработчики: danyowoj & opepl.</p>"
        );

    label->setWordWrap(true);
    layout->addWidget(label);

    QPushButton *okBtn = new QPushButton("OK");
    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(okBtn, 0, Qt::AlignCenter);
}
