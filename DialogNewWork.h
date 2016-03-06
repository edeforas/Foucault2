#ifndef DialogNewWork_
#define DialogNewWork_

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
    unsigned int get_duration();
    void set_work_type(int iWorkType);
    int get_work_type();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::DialogNewWork *ui;
};

#endif
