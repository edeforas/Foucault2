// this file is covered by the General Public License version 2 or later
// please see GPL.html for more details and licensing issues
// copyright Etienne de Foras ( the author )  mailto: etienne.deforas@gmail.com

#include "Foucault2Defines.h"

#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QtGui>
#include <QPrinter>
#include <QPrintDialog>

#include "DialogNewMirror.h"
#include "DialogNewMeasure.h"
#include "DialogNewUnmaskedMeasure.h"
#include "DialogNewComment.h"
#include "DialogNewWork.h"

#include <cassert>
#include <cmath>

#include "Mirror.h"
#include "MirrorIo.h"
#include "MirrorComment.h"
#include "MirrorWork.h"
#include "MirrorCouderMeasure.h"

#include "TimelineScene.h"
#include "TaskItem.h"

///////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    _pMirror=new Mirror;
    _iLastWorkType=WORK_TYPE_UNDEFINED;

    _bMustSave=false;

    _ts=new TimelineScene;
    _ts->set_mirror(_pMirror);
    ui->viewTimelines->setScene(_ts);
    _iqgi=0;

    ui->viewTimelines->setRenderHint(QPainter::Antialiasing,true);
}
///////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete ui;
    delete _pMirror;
    delete _ts;
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (ask_save_and_action())
        event->accept();
    else
        event->ignore();
}
//////////////////////////////////////////////////////////////////////////////
bool MainWindow::ask_save_and_action()
{
    if (_bMustSave==true)
    {
        int iRet=QMessageBox::question(this,tr("Warning"),tr("Save?"),QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (iRet==QMessageBox::Cancel)
            return false;

        if (iRet==QMessageBox::Yes)
        {
            string sFileName = QFileDialog::getSaveFileName(this,tr("Save Foucault2 File"), ".", tr("Foucault2 Files (*.foucault2)")).toStdString();

            if (sFileName.empty()==false)
            {
                _sFileName=sFileName;
                MirrorIo::save(_pMirror,sFileName);
                return true;
            }
            else
                return false;
        }
        _bMustSave=false;
    }

    return true;
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::clear_mirror()
{
    assert(_bMustSave==false);

    delete _pMirror;
    _pMirror=new Mirror;
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionQuit_triggered()
{
    if (ask_save_and_action())
        qApp->quit();
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox mb;
    QString qsText=QString("Foucault2 ");
    qsText+=FOUCAULT2_VERSION;
    qsText+= tr(" by Etienne de Foras");
    qsText+="\nemail: etienne.deforas@gmail.com";
    qsText+= "\n\n";
    qsText+= tr("Thanks to:");
    qsText+= "\n";
    qsText+= tr(" Jean-Jacques Jourdanneau for his help");
    qsText+= "\n";
    qsText+= tr(" Ken Slater for the translations fixes");
    qsText += "\n";
    qsText += tr(" Paul Crubille for all the mirrors image processing and dialogs");

    mb.setText(qsText);
    mb.setIconPixmap(QPixmap(":/Foucault2/Foucault2.ico"));
    mb.exec();
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionLoad_triggered()
{
    if (ask_save_and_action()==false)
        return;

    _sFileName = QFileDialog::getOpenFileName(this,tr("Open Foucault2 File"), ".", tr("Foucault2 Files (*.foucault2)")).toStdString();

    if (!_sFileName.empty())
    {
        clear_mirror();
        device_changed(false);
        load_file(_sFileName);
    }
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionNew_triggered()
{
    if (ask_save_and_action()==false)
        return;

    DialogNewMirror nm;
    if(nm.exec())
    {
        _sFileName="";
        clear_mirror();

        _pMirror->set_name(nm.get_name());
        _pMirror->set_diameter(nm.get_diameter());
        _pMirror->set_hole_diameter(nm.get_hole_diameter());
        _pMirror->set_obstruction_size(nm.get_obstruction_size());
        _pMirror->set_edge_mask_width(nm.get_edge_mask_width());
        _pMirror->set_focal(nm.get_focal());
        _pMirror->set_conical(nm.get_conical());
        _pMirror->set_hx(nm.get_hx());
        _pMirror->set_is_slit_moving(nm.is_slit_moving());

        _pMirror->initialize();

        _bMustSave=true;

        _iLastWorkType=WORK_TYPE_UNDEFINED;
        device_changed(true);
        _ts->update_items(-1);
        ensure_visible(-1);
    }
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionSave_triggered()
{
    if(_sFileName.empty())
        on_actionSave_as_triggered();
    else
    {
        MirrorIo::save(_pMirror,_sFileName);

        _bMustSave=false;
        device_changed(false); // TODO: do not scroll to the start
    }
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionSave_as_triggered()
{
    assert(_pMirror!=0);

    string sFileName = QFileDialog::getSaveFileName(this,tr("Save Foucault2 File"), ".", tr("Foucault2 Files (*.foucault2)")).toStdString();

    if (sFileName.empty()==false)
    {
        _sFileName=sFileName;
        MirrorIo::save(_pMirror,sFileName);
        _bMustSave=false;
        device_changed(false); // TODO: do not scroll to the start
    }
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionClose_triggered()
{
    if (ask_save_and_action()==false)
        return;

    _sFileName="";
    clear_mirror();
    device_changed(false);
}
//////////////////////////////////////////////////////////////////////////////
bool MainWindow::load_file(string sFile)
{
    _sFileName="";
    _bMustSave=false;
    Mirror* pMirror;

    if(sFile.substr(sFile.size()-4)==".mir")
        pMirror=  MirrorIo::import(sFile);
    else
        pMirror=MirrorIo::load(sFile);

    if (pMirror==0)
    {
        QMessageBox m;
        m.setText(tr("Unable to Open file!!"));
        m.exec();
        return false;
    }

    delete _pMirror;
    _pMirror=pMirror;

    _sFileName=sFile;
    device_changed(false);
    ensure_visible(-1);
    return true;
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::device_changed(bool bMustSave)
{
    if (bMustSave)
        _bMustSave=true;

    _ts->set_mirror(_pMirror);

    ui->actionNew_Couder_Measure->setEnabled( _pMirror->nb_zones() !=0);
    ui->actionNew_Unmasked_Measure->setEnabled( _pMirror->diameter() !=0);
    ui->actionAdd_comment->setEnabled( _pMirror->nb_zones()!=0);
    ui->actionWork->setEnabled( _pMirror->nb_zones()!=0);
    ui->actionShow_mirror_both_side->setChecked(_pMirror->get_show_both_side());
    ui->actionShow_TAf_ro->setChecked(_pMirror->get_show_lf_ro());
    ui->actionShow_Colors->setChecked(_pMirror->get_show_colors());
    ui->actionShow_smooth_curves->setChecked(_pMirror->get_smooth_curves());
    ui->actionDiscard_last_task->setEnabled(_pMirror->nb_zones()!=0);

    ui->actionDisplay_Compact->setChecked(_pMirror->get_display_mode()==DISPLAY_MODE_COMPACT);
    ui->actionDisplay_Normal->setChecked(_pMirror->get_display_mode()==DISPLAY_MODE_NORMAL);
    ui->actionDisplay_Details->setChecked(_pMirror->get_display_mode()==DISPLAY_MODE_DETAIL);
    ui->actionDisplay_Full->setChecked(_pMirror->get_display_mode()==DISPLAY_MODE_FULL);

    update_title();

    //compute last work type
    _iLastWorkType=WORK_TYPE_UNDEFINED;
    for(int i=_pMirror->nb_item()-1;i>=0;i--)
    {
        MirrorItem* mi=_pMirror->get_item(i);
        if(mi->type()=="MirrorWork")
        {
            _iLastWorkType=static_cast<MirrorWork*>(mi)->work_type();
            break;
        }
    }
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::update_title()
{
    QString sTitle="Foucault2";
    if (!_sFileName.empty())
        sTitle+=QString(tr(" [loaded "))+_sFileName.c_str()+ "]";

    if (_bMustSave==true)
        sTitle+="  *";

    setWindowTitle(sTitle);
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionAdd_comment_triggered()
{
    DialogNewComment nm;
    if(nm.exec())
    {
        MirrorComment* t=new MirrorComment(nm.get_comment());
        t->set_when(nm.get_when());
        _pMirror->add_item(t);
        _ts->update_items(_pMirror->nb_item()-1);
        _ts->ensure_last_visible();
        _bMustSave=true;
        update_title();
    }
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionNew_Couder_Measure_triggered()
{
    DialogNewMeasure nm(_pMirror,this);
    if(nm.exec())
    {
        MirrorCouderMeasure* t=new MirrorCouderMeasure(_pMirror);
	
	vector<double> vdHm;
	vdHm.resize(_pMirror->nb_zones());
	vdHm[0]=(_pMirror->hole_diameter() + _pMirror->hx()[0])/2.;	    
	for(unsigned int i=1;i< _pMirror->nb_zones() ;i++)
	  vdHm[i]= (_pMirror->hx()[i-1] + _pMirror->hx()[i] )/2;
	
	t->set_measure(nm.get_measure(), vdHm, nm.get_aspect());
	
        t->set_when(nm.get_when());
        _pMirror->add_item(t);
        _ts->update_items(_pMirror->nb_item()-1);
        _ts->ensure_last_visible();
        _bMustSave=true;
        update_title();
    }
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionNew_Unmasked_Measure_triggered()
{
  DialogNewUnmaskedMeasure nm(_pMirror, this);
  printf("unmasked triggered\n");
  if(nm.exec())
    {
      _ts->update_items(_pMirror->nb_item()-1);
      _ts->ensure_last_visible();
      _bMustSave=true;
    };

}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionNew_Couder_Screen_triggered()
{ }
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionNew_Hartmann_Screen_triggered()
{ }
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionAdd_Hartmann_Measure_triggered()
{ }
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionGlobal_settings_triggered()
{}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionPrint_triggered()
{
    //TODO move code in Timeline scene

    //code is not very clean, nor optimized, but works.

    // compute nb of pages
    double dPageRatio=1.414213562373095;
    const vector<TaskItem*>& vti=_ts->items();
    vector<int> viPagesFirstItem;
    double dPageFill=0;
    for(unsigned int i=0;i<vti.size();i++)
    {
        if(dPageFill==0)
            viPagesFirstItem.push_back(i);

        const TaskItem* pti=vti[i];
        dPageFill+=pti->rect().height();
        double dPageHeight=pti->rect().width()*dPageRatio; //all the items have the same width
        if(dPageFill>=dPageHeight)
        {
            // make new page
            dPageFill=pti->rect().height();
            viPagesFirstItem.push_back(i);
        }
    }

    int iNbPages=(int)(viPagesFirstItem.size());
    QPrinter printer;
    printer.setFromTo(1,iNbPages);

    if (QPrintDialog(&printer).exec() == QDialog::Accepted)
    {
        int iFirstPage=printer.fromPage();
        int iLastPage=printer.toPage();

        if(iFirstPage==0)
        {
            iFirstPage=1;
            iLastPage=iNbPages;
        }

        QPainter painter(&printer);
        painter.fillRect(painter.window(), QColor(255, 255, 255, 0)); // to fix a QPainter Bug with alpha channel
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        for(int iPage=iFirstPage;iPage<=iLastPage;iPage++)
        {
            int iFirstItemInPage=viPagesFirstItem[iPage-1];
            int iLastItemInpage;

            if((unsigned int)(iPage-1)==viPagesFirstItem.size()-1) //last page?
          iLastItemInpage=(int)vti.size()-1; // Modif Paul Crubillé // iLastItemInpage=viPagesFirstItem.back(); // Print only one set on the last page
            else
                iLastItemInpage=viPagesFirstItem[iPage]-1;

            double pageStart=vti[iFirstItemInPage]->pos().y();
            double pageHeight=vti[iLastItemInpage]->pos().y()+vti[iLastItemInpage]->rect().height()-pageStart;

            QRectF qrfPage(vti[iFirstItemInPage]->pos().x(),pageStart,vti[iFirstItemInPage]->rect().width(),pageHeight);

            //compute page rect
            QRectF qrfPrinter=printer.pageRect(QPrinter::Millimeter);
            double dRatio=(210.-10.)/210.; //10mm lateral edge on A4
            double dLateralEdge=qrfPrinter.width()-qrfPrinter.width()*dRatio;
            double dVerticalEdge=qrfPrinter.height()-qrfPrinter.height()*dRatio;
            QRectF qrfPrinterBorder(qrfPrinter.left()+dLateralEdge,qrfPrinter.top()+dVerticalEdge,qrfPrinter.width()-2*dLateralEdge,qrfPrinter.height()-2*dVerticalEdge);

            //print!
            _ts->render(&painter,qrfPrinterBorder,qrfPage);

            if (iPage != iLastPage)
                printer.newPage();
        }

        painter.end();
    }
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionImport_triggered()
{
    if (ask_save_and_action()==false)
        return;

    string sFile=QFileDialog::getOpenFileName(this,tr("Import Foucault .mir File"), ".", tr("Foucault Files (*.mir)")).toStdString();

    if (!sFile.empty())
    {
        clear_mirror();
        device_changed(false);

        Mirror* pMirror=MirrorIo::import(sFile);

        if (pMirror==0)
        {
            QMessageBox m;
            m.setText(tr("Unable to Import file!!"));
            m.exec();
            return;
        }

        _bMustSave=true;
        delete _pMirror;
        _pMirror=pMirror;

        //TODO check the file doesn't exist with new extention, ask to overwrite on save

        if(sFile.substr(sFile.size()-4)==".mir")
            _sFileName=sFile.substr(0,sFile.size()-4)+".foucault2";
        else
            _sFileName=sFile+".foucault2";

        device_changed(true);
        ensure_visible(-1);
    }
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionShow_mirror_both_side_triggered()
{
    _pMirror->set_show_both_side(ui->actionShow_mirror_both_side->isChecked());
    device_changed(false);
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionShow_TAf_ro_triggered()
{
    _pMirror->set_show_lf_ro(ui->actionShow_TAf_ro->isChecked());
    device_changed(false);
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionWork_triggered()
{
    DialogNewWork nm;
    nm.set_work_type(_iLastWorkType);
    if(nm.exec())
    {
        MirrorWork* t=new MirrorWork(nm.get_work());
        t->set_when(nm.get_when());
        t->set_duration(nm.get_duration());
        t->set_work_type(nm.get_work_type());

        _pMirror->add_item(t);
        _ts->update_items(_pMirror->nb_item()-1);

        _iLastWorkType=nm.get_work_type();
        _ts->ensure_last_visible();

        _bMustSave=true;
        update_title();
    }
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::ensureVisibleSlot()
{
    _ts->ensure_visible(_iqgi);
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::ensure_visible(int iItem)
{
    // must use a signal: qt bug : https://bugreports.qt.io/browse/QTBUG-5926
    _iqgi=iItem;
    QTimer::singleShot(0,this,SLOT(ensureVisibleSlot()));
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionShow_Colors_triggered()
{
    _pMirror->set_show_colors(ui->actionShow_Colors->isChecked());
    device_changed(false);
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionDisplay_Compact_triggered()
{
    _pMirror->set_display_mode(DISPLAY_MODE_COMPACT);
    device_changed(false);
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionDisplay_Normal_triggered()
{
    _pMirror->set_display_mode(DISPLAY_MODE_NORMAL);
    device_changed(false);
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionDisplay_Details_triggered()
{
    _pMirror->set_display_mode(DISPLAY_MODE_DETAIL);
    device_changed(false);
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionShow_smooth_curves_triggered()
{
    _pMirror->set_smooth_curves(ui->actionShow_smooth_curves->isChecked());
    device_changed(false);
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionZoom_in_triggered()
{
    _ts->zoom_in();
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionZoom_out_triggered()
{
    _ts->zoom_out();
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionDiscard_last_task_triggered()
{
    if(QMessageBox::question(this,tr("Warning"),tr("Delete last task?"),QMessageBox::Yes | QMessageBox::No )==QMessageBox::No)
        return;

    _pMirror->remove_last_item();
    device_changed(true);
    _ts->ensure_last_visible();
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionDisplay_Full_triggered()
{
    _pMirror->set_display_mode(DISPLAY_MODE_FULL);
    device_changed(false);
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::resizeEvent( QResizeEvent *e )
{
    (void)e;

    _ts->ensure_visible(-1);
}
//////////////////////////////////////////////////////////////////////////////
