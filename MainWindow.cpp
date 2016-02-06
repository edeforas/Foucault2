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
#include "DialogNewComment.h"
#include "DialogNewWork.h"

#include <cassert>

#include "Mirror.h"
#include "MirrorIo.h"
#include "MirrorComment.h"
#include "MirrorWork.h"
#include "MirrorCouderMeasure.h"

#include "TimelineScene.h"
///////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    _pMirror=new Mirror;

    _bMustSave=false;

    _ts=new TimelineScene;
    _ts->set_mirror(_pMirror);
    ui->viewTimelines->setScene(_ts);
    _iqgi=0;
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
    mb.setText((QString("Foucault2 ")+FOUCAULT2_VERSION+ tr(" by Etienne de Foras\n\nmail: etienne.deforas@gmail.com")));
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
        clear_mirror();

        _pMirror->set_name(nm.get_name());
        _pMirror->set_diameter(nm.get_diameter());
        _pMirror->set_hole_diameter(nm.get_hole_diameter());
        _pMirror->set_focal(nm.get_focal());
        _pMirror->set_conical(nm.get_conical());
        _pMirror->set_hx(nm.get_hx());
        _pMirror->set_is_slit_moving(nm.is_slit_moving());

        _pMirror->initialize();

        _bMustSave=true;

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
        device_changed(false);
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
        device_changed(false);
    }
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionClose_triggered()
{
    if (ask_save_and_action()==false)
        return;

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
        _bMustSave=true; //todo remove?

    _ts->set_mirror(_pMirror);

    ui->actionNew_Couder_Measure->setEnabled(_pMirror->nb_zones()!=0);
    ui->actionAdd_comment->setEnabled(_pMirror->nb_zones()!=0);
    ui->actionWork->setEnabled(_pMirror->nb_zones()!=0);
    ui->actionShow_mirror_both_side->setChecked(_pMirror->get_show_both_side());
    ui->actionShow_Colors->setChecked(_pMirror->get_show_colors());
    ui->actionShow_smooth_curves->setChecked(_pMirror->get_smooth_curves());

    ui->actionDisplay_Compact->setChecked(_pMirror->get_display_mode()==0);
    ui->actionDisplay_Normal->setChecked(_pMirror->get_display_mode()==1);
    ui->actionDisplay_Details->setChecked(_pMirror->get_display_mode()==2);

    update_title();
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
        _pMirror->add_item(t);
        _ts->update_items(_pMirror->nb_item()-1);
        ensure_visible(_pMirror->nb_item()-1);
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
        t->set_measure(nm.get_measure(),nm.get_aspect());
        _pMirror->add_item(t);
        _ts->update_items(_pMirror->nb_item()-1);
        ensure_visible(_pMirror->nb_item()-1);
        _bMustSave=true;
        update_title();
    }
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
    QPrinter printer;
    if (QPrintDialog(&printer).exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        painter.fillRect(painter.window(), QColor(255, 255, 255, 0)); // to fix a QPainter Bug with alpha channel

        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);

        _ts->render(&painter);
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

        _sFileName=sFile+".foucault2";//TODO remove old extention?
        device_changed(true);
        ensure_visible(-1);
    }
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionShow_mirror_both_side_triggered()
{
    bool bShowBothSide=ui->actionShow_mirror_both_side->isChecked();
    _pMirror->set_show_both_side(bShowBothSide);
    device_changed(false);
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionWork_triggered()
{
    DialogNewWork nm;
    if(nm.exec())
    {
        MirrorWork* t=new MirrorWork(nm.get_work());
        _pMirror->add_item(t);
        _ts->update_items(_pMirror->nb_item()-1);

        ensure_visible(_pMirror->nb_item()-1);

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
    _pMirror->set_display_mode(0);
    device_changed(false);
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionDisplay_Normal_triggered()
{
    _pMirror->set_display_mode(1);
    device_changed(false);
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionDisplay_Details_triggered()
{
    _pMirror->set_display_mode(2);
    device_changed(false);
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionShow_smooth_curves_triggered()
{
    _pMirror->set_smooth_curves(ui->actionShow_smooth_curves->isChecked());
    device_changed(false);
}
///////////////////////////////////////////////////////////////////////////////
