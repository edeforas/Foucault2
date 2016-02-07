#ifndef DIALOGNEWMEASURE_H
#define DIALOGNEWMEASURE_H

#include <QDialog>

#include <vector>
#include <string>
using namespace std;

class Mirror;

namespace Ui {
class DialogNewMeasure;
}

class DialogNewMeasure : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewMeasure(Mirror *pMirror, QWidget *parent = 0);
    ~DialogNewMeasure();

    vector<double> get_measure();
    string get_aspect();
    unsigned int get_when();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::DialogNewMeasure *ui;
    Mirror* _pMirror;

    vector<double> _vdMeasures;
};

#endif // DIALOGNEWMEASURE_H
