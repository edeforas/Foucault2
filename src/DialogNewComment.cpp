#include "DialogNewComment.h"
#include "ui_DialogNewComment.h"

DialogNewComment::DialogNewComment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewComment)
{
    ui->setupUi(this);
    ui->pleComment->setFocus();

    ui->dteWhen->setDate(QDate::currentDate());
    ui->dteWhen->setTime(QTime::currentTime());
}

DialogNewComment::~DialogNewComment()
{
    delete ui;
}

void DialogNewComment::on_pushButton_clicked()
{
    accept();
}

void DialogNewComment::on_pushButton_2_clicked()
{
    reject();
}

string DialogNewComment::get_comment()
{
    return ui->pleComment->toPlainText().toStdString();
}

unsigned int DialogNewComment::get_when()
{
    return (unsigned int)ui->dteWhen->dateTime().toSecsSinceEpoch();
}
