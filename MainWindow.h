// this file is covered by the General Public License version 2 or later
// please see GPL.html for more details and licensing issues
// copyright Etienne de Foras ( the author )  mailto: etienne.deforas@gmail.com

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsItem>

#include <string>
using namespace std;

namespace Ui
{
    class MainWindowClass;
}

class Mirror;
class TimelineScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool load_file(string sFile);
    bool ask_save_and_action();
    void clear_mirror();

public slots:
    void device_changed(bool bMustSave);

protected:
    virtual void closeEvent ( QCloseEvent * event );

private:
    Ui::MainWindowClass *ui;

private slots:
    void on_actionImport_triggered();
    void on_actionPrint_triggered();
    void on_actionGlobal_settings_triggered();
    void on_actionAdd_Hartmann_Measure_triggered();
    void on_actionNew_Hartmann_Screen_triggered();
    void on_actionNew_Couder_Screen_triggered();
    void on_actionNew_Couder_Measure_triggered();
    void on_actionAdd_comment_triggered();
    void on_actionNew_triggered();
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_actionClose_triggered();
    void on_actionAbout_triggered();
    void on_actionQuit_triggered();
    void on_actionShow_mirror_both_side_triggered();
    void on_actionWork_triggered();
    void ensureVisibleSlot();
    void on_actionShow_Colors_triggered();
    void on_actionDisplay_Compact_triggered();
    void on_actionDisplay_Normal_triggered();
    void on_actionDisplay_Details_triggered();
    void on_actionShow_smooth_curves_triggered();

private:
    void update_title();
    void ensure_visible(int iItem);
    Mirror* _pMirror;

    bool _bMustSave;
    string _sFileName;
    TimelineScene* _ts;
    int _iqgi;
};

#endif // MAINWINDOW_H
