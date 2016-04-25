#ifndef TimelineScene_
#define TimelineScene_

#include <QGraphicsScene>
#include <vector>
using namespace std;

class TaskItem;
class MirrorItem;
class Mirror;

#define SCENE_BOUNDARIES 60

class TimelineScene : public QGraphicsScene
{
public:
    TimelineScene();

    void set_mirror(Mirror* pM);

    void update_items(int iFirstItem=-1);
    void ensure_visible(int iItem);
    void ensure_last_visible();

    void zoom_in();
    void zoom_out();

    const vector<TaskItem*>& items() const;

protected:
    virtual void wheelEvent(QGraphicsSceneWheelEvent* wheelEvent);

private:
    void add_item(TaskItem* pi);
    TaskItem* create_item(MirrorItem* rmi);

    Mirror *_pM;
    vector<TaskItem*> _vti;
};

#endif
