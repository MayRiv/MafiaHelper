#include "warningbutton.h"

WarningButton::WarningButton(QWidget *parent) :
    QPushButton(parent)
{
    warningNumber = 0;
}

void WarningButton::increaseWarnings()
{
    warningNumber++;
    this->setText(QString(warningNumber));
    if (warningNumber > 3)
    {
        this->setEnabled(false);
        emit scored4warnings();
    }
}
