#include "affdialog.h"
#include "ui_affdialog.h"

affDialog::affDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::affDialog)
{
    ui->setupUi(this);
}

affDialog::~affDialog()
{
    delete ui;
}

void affDialog::on_buttonBox_accepted()
{
    degree = ui->lineEdit->text().toDouble();
    tx = ui->lineEdit_2->text().toDouble();
    ty = ui->lineEdit_3->text().toDouble();
    size = ui->lineEdit_4->text().toDouble();
    shear = ui->lineEdit_5->text().toDouble();

    this->close();
}


void affDialog::on_buttonBox_rejected()
{
    degree = 0;
    tx = 0;
    ty = 0;
    size = 1;
    shear = 0;

    this->close();
}

