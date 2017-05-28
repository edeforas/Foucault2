#include "DialogNewMeasure.h"
#include "ui_DialogNewMeasure.h"

#include "Mirror.h"

#include <cassert>

#include <QMessageBox>

DialogNewMeasure::DialogNewMeasure(Mirror* pMirror,QWidget *parent):
    QDialog(parent),
    ui(new Ui::DialogNewMeasure)
{
    assert(pMirror);
    assert(pMirror->nb_zones()!=0);

    ui->setupUi(this);

    _pMirror=pMirror;

    ui->twZones->setColumnCount(_pMirror->nb_zones());
    ui->twZones->horizontalHeader()->setVisible(true);

    for(unsigned int i=0;i<_pMirror->nb_zones();i++)
        ui->twZones->setItem(0,i,new QTableWidgetItem());

    ui->twZones->item(0,0)->setSelected(true);

    ui->dteWhen->setDate(QDate::currentDate());
    ui->dteWhen->setTime(QTime::currentTime());
}

DialogNewMeasure::~DialogNewMeasure()
{
    delete ui;
}

void DialogNewMeasure::on_pushButton_2_clicked()
{
    vector<double> vdMes;
    int iError=-1;

    for(unsigned int i=0;i<_pMirror->nb_zones();i++)
    {
        QTableWidgetItem* pItem=ui->twZones->item(0,i);
        assert(pItem);

        QString sTxt=pItem->text();
        bool bOk;
        vdMes.push_back(sTxt.toDouble(&bOk));
        if(!bOk)
        {
            iError=i;
            break;
        }
    }

    if(iError!=-1)
    {
        for(unsigned int i=0;i<_pMirror->nb_zones();i++)
            ui->twZones->item(0,i)->setSelected(false);

        ui->twZones->setFocus();
        QMessageBox::warning(this,tr("Error"),tr("Please, enter a valid number at measure #")+QString::number(iError+1));
        return;
    }

    _vdMeasures=vdMes;

    assert((unsigned int)_vdMeasures.size()==_pMirror->nb_zones());
    accept();
}

void DialogNewMeasure::on_pushButton_clicked()
{
    reject();
}

vector<double> DialogNewMeasure::get_measure()
{
    return _vdMeasures;
}

string DialogNewMeasure::get_aspect()
{
    return ui->leAspect->text().toStdString();
}

unsigned int DialogNewMeasure::get_when()
{
    return ui->dteWhen->dateTime().toTime_t();
}
