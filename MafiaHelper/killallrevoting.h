#ifndef KILLALLREVOTING_H
#define KILLALLREVOTING_H

#include <QDialog>

namespace Ui {
class KillAllRevoting;
}

class KillAllRevoting : public QDialog
{
    Q_OBJECT

public:
    explicit KillAllRevoting(QWidget *parent = 0);
    ~KillAllRevoting();

private:
    Ui::KillAllRevoting *ui;
};

#endif // KILLALLREVOTING_H
