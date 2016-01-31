#ifndef TimelineScene_
#define TimelineScene_

#include <QGraphicsScene>
#include <vector>
using namespace std;

class TaskItem;
class MirrorItem;
class Mirror;

class TimelineScene : public QGraphicsScene
{
public:
    TimelineScene();

    void set_mirror(Mirror* pM);

    void update_items(int iFirstItem=-1);
    void ensure_visible(int iItem);

protected:
    virtual void wheelEvent(QGraphicsSceneWheelEvent* wheelEvent);
    virtual void keyPressEvent(QKeyEvent * keyEvent);

private:
    void add_item(TaskItem* pi);
    TaskItem* create_item(MirrorItem* rmi);

    Mirror *_pM;

    vector<TaskItem*> _vti;
};

#endif
