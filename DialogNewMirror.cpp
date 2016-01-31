#include "DialogNewMirror.h"
#include "ui_DialogNewMirror.h"

#include <QMessageBox>
#include "Mirror.h"
//////////////////////////////////////////////////////////////////////////////
DialogNewMirror::DialogNewMirror(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewMirror)
{
    ui->setupUi(this);
    ui->twHx->horizontalHeader()->setVisible(true);

    _dDiameter=0;
    _dHoleDiameter=0;
    _dFocal=0;
    _dConical=0;
    _bMovingSlit=false;
}
//////////////////////////////////////////////////////////////////////////////
DialogNewMirror::~DialogNewMirror()
{
    delete ui;
}
/////////////////////////////////////////////////////////////////////////////////
bool DialogNewMirror::get_and_check_DFH()
{
    bool bSuccess;

    _dDiameter=ui->leOpticalDiameter->text().toDouble(&bSuccess);
    if((!bSuccess) || (_dDiameter<=0.) )
    {
        ui->leOpticalDiameter->selectAll();
        QMessageBox::critical(this,tr("Error"),tr("Please enter a valid diameter"));
        ui->leOpticalDiameter->setFocus();
        return false;
    }

    if(_dDiameter<50)
    {
        if(QMessageBox::warning(this,tr("Diameter is small"),tr("Diameter is under 50 mm, is it intentional?"),QMessageBox::Yes|QMessageBox::No)==QMessageBox::No)
        {
            ui->leOpticalDiameter->selectAll();
            ui->leOpticalDiameter->setFocus();
            return false;
        }
    }

    _dHoleDiameter=ui->leHoleDiameter->text().toDouble(&bSuccess);
    if((!bSuccess) || (_dHoleDiameter<0.) )
    {
        ui->leHoleDiameter->selectAll();
        QMessageBox::critical(this,tr("Error"),tr("Please enter a valid hole diameter"));
        ui->leHoleDiameter->setFocus();
        return false;
    }

    if(_dHoleDiameter>=_dDiameter)
    {
        ui->leHoleDiameter->selectAll();
        QMessageBox::critical(this,tr("Error"),tr("Hole diameter is bigger than the diameter"));
        ui->leHoleDiameter->setFocus();
        return false;
    }

    if(_dHoleDiameter*2.>=_dDiameter)
    {
        if(QMessageBox::warning(this,tr("Hole Diameter is big"),tr("Hole Diameter is bigger than 50% of the mirror diameter, is it intentional?"),QMessageBox::Yes|QMessageBox::No)==QMessageBox::No)
        {
            ui->leOpticalDiameter->selectAll();
            ui->leOpticalDiameter->setFocus();
            return false;
        }
    }

    _dFocal=ui->leFocal->text().toDouble(&bSuccess);
    if((!bSuccess) || (_dFocal<=0.) )
    {
        ui->leFocal->selectAll();
        QMessageBox::critical(this,tr("Error"),tr("Please enter a valid focal"));
        ui->leFocal->setFocus();
        return false;
    }

    if(_dFocal<_dDiameter*3.)
    {
        if(QMessageBox::warning(this,tr("Focal is short"),tr("Focal is short, compared to the diameter, is it intentional?"),QMessageBox::Yes|QMessageBox::No)==QMessageBox::No)
        {
            ui->leFocal->selectAll();
            ui->leFocal->setFocus();
            return false;
        }
    }

    return true;
}
/////////////////////////////////////////////////////////////////////////////////
void DialogNewMirror::on_pushButton_2_clicked()
{
    bool bSuccess=true;

    _sName=ui->leName->text().toStdString();

    if(!get_and_check_DFH())
        return;

    _dConical=ui->leConicalConstant->text().toDouble(&bSuccess);
    if((!bSuccess) )
    {
        ui->leConicalConstant->selectAll();
        QMessageBox::critical(this,tr("Error"),tr("Please enter a valid conical constant (-1 for parabolic, 0 for a sphere, other value for aspheric)"));
        ui->leConicalConstant->setFocus();
        return;
    }

    if( (_dConical>6.) || (_dConical<-6.) )
    {
        if(QMessageBox::warning(this,tr("Conical constant is high"),tr("Mirror is strongly aspheric due to conical constant, is it intentional?"),QMessageBox::Yes|QMessageBox::No)==QMessageBox::No)
        {
            ui->leConicalConstant->selectAll();
            ui->leConicalConstant->setFocus();
            return;
        }
    }

    if(ui->cbSlitType->currentIndex()==0)
    {
        QMessageBox::critical(this,tr("Error"),tr("Please select a slit type: moving or not (with the foucaultmeter knife)"));
        ui->cbSlitType->setFocus();
        return;
    }

    _bMovingSlit=ui->cbSlitType->currentIndex()==2;

    _dHx.clear();
    double dLastHx=_dHoleDiameter/2.;

    //unselect all items
    for(int i=0;i<10;i++)
    {
        QTableWidgetItem* pItem=ui->twHx->item(0,i);
        if(pItem)
            pItem->setSelected(false);
    }

    for(int i=0;i<10;i++)
    {
        QTableWidgetItem* pItem=ui->twHx->item(0,i);
        if(pItem)
        {
            if(pItem->text().size()==0)
                break;

            double dHxTmp=pItem->text().toDouble(&bSuccess);
            if(!bSuccess && (pItem->text().size()!=0) )
            {
                pItem->setSelected(true);
                QMessageBox::critical(this,tr("Error"),tr("Please enter a valid Hx (i.e. a numerical value)"));
                return;
            }

            if( (dHxTmp<=dLastHx) && (i==0) )
            {
                pItem->setSelected(true);
                QMessageBox::critical(this,tr("Error"),tr("Hx is lower than the mirror hole radius"));
                return;
            }

            if( (dHxTmp<=dLastHx) )
            {
                pItem->setSelected(true);
                QMessageBox::critical(this,tr("Error"),tr("Hx is lower than or equal the previous Hx"));
                return;
            }

            if( (dHxTmp-0.001>_dDiameter/2.))
            {
                pItem->setSelected(true);
                QMessageBox::critical(this,tr("Error"),tr("Hx is bigger than the mirror radius"));
                return;
            }

            dLastHx=dHxTmp;
            _dHx.push_back(dHxTmp);
        }
        else
            break;
    }

    if(_dHx.empty())
    {
        QMessageBox::critical(this,tr("Error"),tr("Hx are empty"));
        return;
    }

    if(_dHx.size()<3)
    {
        QMessageBox::critical(this,tr("Error"),tr("Too few Hx (under 3 values), please add more Hx"));
        return;
    }

    if(_dHx[_dHx.size()-1]+0.001<_dDiameter/2.)
    {
        ui->twHx->item(0,_dHx.size()-1)->setSelected(true);
        QMessageBox::critical(this,tr("Error"),tr("Last Hx must be equal to the mirroir radius (i.e. the diameter/2)"));
        return;
    }

    accept();
}
/////////////////////////////////////////////////////////////////////////////////
void DialogNewMirror::on_pushButton_clicked()
{
    reject();
}
//////////////////////////////////////////////////////////////////////////////
string DialogNewMirror::get_name()
{
    return _sName;
}
//////////////////////////////////////////////////////////////////////////////
double DialogNewMirror::get_diameter()
{
    return _dDiameter;
}
//////////////////////////////////////////////////////////////////////////////
double DialogNewMirror::get_hole_diameter()
{
    return _dHoleDiameter;
}
//////////////////////////////////////////////////////////////////////////////
double DialogNewMirror::get_focal()
{
    return _dFocal;
}
//////////////////////////////////////////////////////////////////////////////
double DialogNewMirror::get_conical()
{
    return _dConical;
}
//////////////////////////////////////////////////////////////////////////////
bool DialogNewMirror::is_slit_moving()
{
    return _bMovingSlit;
}
//////////////////////////////////////////////////////////////////////////////
vector<double> DialogNewMirror::get_hx()
{
    return _dHx;
}
//////////////////////////////////////////////////////////////////////////////
void DialogNewMirror::on_btnComputeNbOfzone_clicked()
{
    // compute with number of zones
    if(!get_and_check_DFH())
        return;

    bool bSuccess;
    int iNbZone=ui->leNumberOfZones->text().toInt(&bSuccess);

    if((!bSuccess) || (iNbZone>10) || (iNbZone<3) )
    {
        ui->leNumberOfZones->setFocus();
        QMessageBox::critical(this,tr("Error"),tr("please enter a valid number of zone (must be <10 and >=3)"));
        return;
    }

    vector<double> vdHx;
    Mirror::design_hx_nb_zone(_dDiameter,_dHoleDiameter,_dFocal,iNbZone, vdHx);

    for(unsigned int i=0;i<vdHx.size();i++)
    {
        QTableWidgetItem* qti =new QTableWidgetItem(QString::number(vdHx[i],'f',1));
        ui->twHx->setItem(0,i,qti);
    }

    for(unsigned int i=vdHx.size();i<10;i++)
    {
        QTableWidgetItem* qti =new QTableWidgetItem("");
        ui->twHx->setItem(0,i,qti);
    }
}
//////////////////////////////////////////////////////////////////////////////
void DialogNewMirror::on_btnComputeNbMillimetersByZone_clicked()
{
    //compute with nb millimeters/zones
    if(!get_and_check_DFH())
        return;

    bool bSuccess;
    double dMMByZone=ui->leMillimetersByZone->text().toDouble(&bSuccess);

    if( (!bSuccess) || (dMMByZone<=0.) )
    {
        ui->leMillimetersByZone->setFocus();
        QMessageBox::critical(this,tr("Error"),tr("please enter a valid number of mm by zone (must be positive)"));
        return;
    }

    vector<double> vdHx;
    Mirror::design_hx_mm_by_zone(_dDiameter,_dHoleDiameter,_dFocal,dMMByZone, vdHx);

    for(unsigned int i=0;i<vdHx.size();i++)
    {
        QTableWidgetItem* qti =new QTableWidgetItem(QString::number(vdHx[i],'f',1));
        ui->twHx->setItem(0,i,qti);
    }

    for(unsigned int i=vdHx.size();i<10;i++)
    {
        QTableWidgetItem* qti =new QTableWidgetItem("");
        ui->twHx->setItem(0,i,qti);
    }
}
//////////////////////////////////////////////////////////////////////////////
void DialogNewMirror::on_btnComputeMinZoneSize_clicked()
{
    //compute with min zone size
    if(!get_and_check_DFH())
        return;

    bool bSuccess;
    double dMinZoneSize=ui->leMinZoneSize->text().toDouble(&bSuccess);

    if( (!bSuccess) || (dMinZoneSize<=0.) )
    {
        ui->leMinZoneSize->setFocus();
        QMessageBox::critical(this,tr("Error"),tr("please enter a valid number of minimum mm by zone (must be positive)"));
        return;
    }

    vector<double> vdHx;
    Mirror::design_hx_min_zone_size(_dDiameter,_dHoleDiameter,_dFocal,dMinZoneSize, vdHx);

    for(unsigned int i=0;i<vdHx.size();i++)
    {
        QTableWidgetItem* qti =new QTableWidgetItem(QString::number(vdHx[i],'f',1));
        ui->twHx->setItem(0,i,qti);
    }

    for(unsigned int i=vdHx.size();i<10;i++)
    {
        QTableWidgetItem* qti =new QTableWidgetItem("");
        ui->twHx->setItem(0,i,qti);
    }
}
//////////////////////////////////////////////////////////////////////////////
void DialogNewMirror::on_radioButton_2_clicked()
{
    // select with number of zone
    ui->leNumberOfZones->setEnabled(true);
    ui->leMillimetersByZone->setEnabled(false);
    ui->btnComputeNbOfzone->setEnabled(true);
    ui->btnComputeNbMillimetersByZone->setEnabled(false);
    ui->btnComputeMinZoneSize->setEnabled(false);
    ui->leMinZoneSize->setEnabled(false);
}
//////////////////////////////////////////////////////////////////////////////
void DialogNewMirror::on_radioButton_3_clicked()
{
    //select with nb millimeters/zone
    ui->leNumberOfZones->setEnabled(false);
    ui->leMillimetersByZone->setEnabled(true);
    ui->btnComputeNbOfzone->setEnabled(false);
    ui->btnComputeNbMillimetersByZone->setEnabled(true);
    ui->btnComputeMinZoneSize->setEnabled(false);
    ui->leMinZoneSize->setEnabled(false);
}
//////////////////////////////////////////////////////////////////////////////
void DialogNewMirror::on_radioButton_clicked()
{
    // select with manual design
    ui->leNumberOfZones->setEnabled(false);
    ui->leMillimetersByZone->setEnabled(false);
    ui->btnComputeNbOfzone->setEnabled(false);
    ui->btnComputeNbMillimetersByZone->setEnabled(false);
    ui->btnComputeMinZoneSize->setEnabled(false);
    ui->leMinZoneSize->setEnabled(false);
}
//////////////////////////////////////////////////////////////////////////////
void DialogNewMirror::on_radioButton_4_clicked()
{
    // select with min zone size
    ui->leNumberOfZones->setEnabled(false);
    ui->leMillimetersByZone->setEnabled(false);
    ui->btnComputeNbOfzone->setEnabled(false);
    ui->btnComputeNbMillimetersByZone->setEnabled(false);
    ui->btnComputeMinZoneSize->setEnabled(true);
    ui->leMinZoneSize->setEnabled(true);
}
//////////////////////////////////////////////////////////////////////////////
