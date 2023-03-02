#include "DialogNewWork.h"
#include "ui_DialogNewWork.h"

#include "MirrorWork.h"

DialogNewWork::DialogNewWork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewWork)
{
    ui->setupUi(this);
    ui->pleWork->setFocus();

    ui->dteWhen->setDate(QDate::currentDate());
    ui->dteWhen->setTime(QTime::currentTime());

    ui->cbWorkType->setCurrentIndex(WORK_TYPE_UNDEFINED);
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
    return (unsigned int)ui->dteWhen->dateTime().toSecsSinceEpoch();
}

unsigned int DialogNewWork::get_duration()
{
    return ui->sbHour->value()*3600+ui->sbMinute->value()*60+ui->sbSecond->value();
}

void DialogNewWork::set_work_type(int iWorkType)
{
    ui->cbWorkType->setCurrentIndex(iWorkType);
}

int DialogNewWork::get_work_type()
{
    return ui->cbWorkType->currentIndex();
}
