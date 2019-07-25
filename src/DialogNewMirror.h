#ifndef NEWMIRROR_H
#define NEWMIRROR_H

#include <QDialog>

#include <string>
#include <vector>
using namespace std;

namespace Ui {
class NewMirror;
}

class DialogNewMirror : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewMirror(QWidget *parent = 0);
    ~DialogNewMirror();

    string get_name();
    double get_diameter();
    double get_hole_diameter();
    double get_focal();
    double get_conical();
    vector<double> get_hx();
    bool is_slit_moving();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_3_clicked();
    void on_radioButton_clicked();
    void on_btnComputeNbOfzone_clicked();
    void on_btnComputeNbMillimetersByZone_clicked();
    void on_btnComputeMinZoneSize_clicked();
    void on_radioButton_4_clicked();

private:
    bool get_and_check_DFH();
    Ui::NewMirror *ui;

    string _sName;
    double _dDiameter;
    double _dHoleDiameter;
    double _dFocal;
    double _dConical;
    vector<double> _dHx;
    bool _bMovingSlit;
};

#endif // NEWMIRROR_H
