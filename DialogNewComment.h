#ifndef DIALOGNEWCOMMENT_H
#define DIALOGNEWCOMMENT_H

#include <QDialog>


#include <string>
using namespace std;

namespace Ui {
class DialogNewComment;
}

class DialogNewComment : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewComment(QWidget *parent = 0);
    ~DialogNewComment();

    string get_comment();
    unsigned int get_when();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DialogNewComment *ui;
};

#endif // DIALOGNEWCOMMENT_H
