#include "warningbutton.h"

WarningButton::WarningButton(QWidget *parent) :
    QPushButton(parent)
{
    warningNumber = 0;
    setText(QString("%1").arg(0));
    connect(this,SIGNAL(clicked()),this,SLOT(increaseWarnings()));
}

void WarningButton::increaseWarnings()
{
    warningNumber++;
    this->setText(QString("%1").arg(warningNumber));
    if (warningNumber > 3)
    {
        emit scored4warnings();
    }
}
