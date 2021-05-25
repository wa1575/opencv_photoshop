#ifndef AFFDIALOG_H
#define AFFDIALOG_H

#include <QDialog>

namespace Ui {
class affDialog;
}

class affDialog : public QDialog
{
    Q_OBJECT

public:
    explicit affDialog(QWidget *parent = nullptr);
    ~affDialog();

    double degree;
    double tx, ty;
    double size;
    double shear;

//signals:
  //  void Send_value(int degree, int tx, int ty, double size, double shear);


private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::affDialog *ui;
};

#endif // AFFDIALOG_H
