#ifndef NIGHTDIALOG_H
#define NIGHTDIALOG_H

#include <QDialog>

namespace Ui {
class NightDialog;
}

class NightDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NightDialog(QWidget *parent = 0);
    ~NightDialog();
signals:
    void killed(int);

private slots:
    void on_pushButton_clicked();

private:
    Ui::NightDialog *ui;
};

#endif // NIGHTDIALOG_H
