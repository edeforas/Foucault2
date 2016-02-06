#ifndef TaskItem_
#define TaskItem_

#include <QGraphicsRectItem>

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include <string>
using namespace std;

class MirrorItem;
class TaskItem : public QGraphicsRectItem
{
public:
    TaskItem(MirrorItem *pItem);

    void add_item(QGraphicsItem *pItem);
    void add_line_tab(string sTitle, vector<double> val, float x, float y, float width);
    virtual QRectF boundingRect() const;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void set_background_color(QColor color);

    int block_size();

protected:
    MirrorItem *_pItem;

private:
    QRectF _boundingRect;
    QColor _color;
};

#endif
