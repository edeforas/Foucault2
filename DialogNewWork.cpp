#include "DialogNewWork.h"
#include "ui_DialogNewWork.h"

DialogNewWork::DialogNewWork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewWork)
{
    ui->setupUi(this);
    ui->pleWork->setFocus();

    ui->dteWhen->setDate(QDate::currentDate());
    ui->dteWhen->setTime(QTime::currentTime());
}

DialogNewWork::~DialogNewWork()
{
    delete ui;
}

void DialogNewWork::on_pushButton_clicked()
{
    accept();
}

void DialogNewWork::on_pushButton_2_clicked()
{
    reject();
}

string DialogNewWork::get_work()
{
    return ui->pleWork->toPlainText().toStdString();
}

unsigned int DialogNewWork::get_when()
{
    return ui->dteWhen->dateTime().toTime_t();
}
