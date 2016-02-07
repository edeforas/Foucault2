#ifndef DIALOGNEWWork_H
#define DIALOGNEWWork_H

#include <QDialog>


#include <string>
using namespace std;

namespace Ui {
class DialogNewWork;
}

class DialogNewWork : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewWork(QWidget *parent = 0);
    ~DialogNewWork();

    string get_work();
    unsigned int get_when();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::DialogNewWork *ui;
};

#endif // DIALOGNEWWork_H
