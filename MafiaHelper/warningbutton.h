#ifndef WARNINGBUTTON_H
#define WARNINGBUTTON_H

#include <QPushButton>

class WarningButton : public QPushButton
{
    Q_OBJECT
public:
    explicit WarningButton(QWidget *parent = 0);
    void removeAllWarnings();
signals:
    void scored4warnings();
public slots:
    void increaseWarnings();
private:
    int warningNumber;
};

#endif // WARNINGBUTTON_H
