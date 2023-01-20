#include "TaskItemUnmaskedMeasure.h"
#include "Mirror.h"
// #include "MirrorCouderMeasure.h"
#include "MirrorUnmaskedMeasure.h"
#include "Foucault2Defines.h"

#include <string>
using namespace std;

////////////////////////////////////////////////////////////////////////////
TaskItemUnmaskedMeasure::TaskItemUnmaskedMeasure(MirrorItem* pItem,int iBlockSize):TaskItem(pItem,iBlockSize)
{
    int iLine=pos().y();
    const Mirror* pM=pItem->mirror();
    int iDisplayMode=pM->get_display_mode();

    MirrorUnmaskedMeasure* mui=static_cast<MirrorUnmaskedMeasure*>(pItem);
    if(iDisplayMode>=DISPLAY_MODE_NORMAL)
    {
        string s=mui->when_as_text();
        if(!s.empty())
        {
            QGraphicsTextItem* ptiWhen=new QGraphicsTextItem(s.c_str());
            ptiWhen->setPos(pos().x(),iLine);
            add_item(ptiWhen);
        }
        QGraphicsTextItem* ptiTitleTab=new QGraphicsTextItem("Unmasked photographic foucault measure:");
        ptiTitleTab->setPos(pos().x()+iBlockSize*31,iLine);
        add_item(ptiTitleTab);

        iLine+=iBlockSize;
        QGraphicsTextItem* ptiTotalDiam=new QGraphicsTextItem(QObject::tr("Mirror total diameter: ")
                                                              +QString::number(mui->get_mirror_total_diameter())+QString(" mm"));
        ptiTotalDiam->setPos(pos().x() ,iLine);
        add_item(ptiTotalDiam);

    }
    else
    {
        QGraphicsTextItem* ptiTitleTab=new QGraphicsTextItem("Unmasked photographic foucault measure:");
        ptiTitleTab->setPos(pos().x() ,iLine);
        add_item(ptiTitleTab);
    }

    QGraphicsTextItem* ptinbImg=new QGraphicsTextItem(QObject::tr("Number of images: ")+QString::number(mui->get_number_images())+QString(" "));
    ptinbImg->setPos(pos().x() +iBlockSize*24,iLine);
    add_item(ptinbImg);
    
    QGraphicsTextItem* ptideltaHx=new QGraphicsTextItem(QObject::tr("delta hx between images: ")
                                                        +QString::number(mui->get_delta_hx())+QString(" mm"));
    ptideltaHx->setPos(pos().x() +iBlockSize*42 ,iLine);
    add_item(ptideltaHx);
    
    if(iDisplayMode>=DISPLAY_MODE_NORMAL)
    {
        iLine+=iBlockSize;
        QGraphicsTextItem* ptiImgName=new QGraphicsTextItem( QObject::tr("First image path: ")
                                                             +QString::fromStdString(mui->get_first_image_name()));
        ptiImgName->setPos(pos().x(),iLine);
        add_item(ptiImgName);

        iLine+=iBlockSize;

    }

    if(iDisplayMode>=DISPLAY_MODE_DETAIL)
    {
        iLine+=iBlockSize;
        QGraphicsTextItem* ptiFlatName=new QGraphicsTextItem( QObject::tr("Flat image path: ")
                                                              +QString::fromStdString(mui->get_flat_image_name()));
        ptiFlatName->setPos(pos().x(),iLine);
        add_item(ptiFlatName);

        iLine+=iBlockSize;

        QGraphicsTextItem* ptiSlit=new QGraphicsTextItem(QObject::tr("Slit angle: ")
                                                         +QString::number(mui->get_slit_angle())+QString("°"));
        ptiSlit->setPos(pos().x() ,iLine);
        add_item(ptiSlit);

        QGraphicsTextItem* ptiChannel=new QGraphicsTextItem(QObject::tr("Channel: ")
                                                            +QString::number(mui->get_RGB_channel())+QString(" (0 (Gray), 1(Red), 2(Green), 3(Blue)) "));
        ptiChannel->setPos(pos().x() +iBlockSize*24 ,iLine);
        add_item(ptiChannel);

    }

    if(iDisplayMode > DISPLAY_MODE_DETAIL)
    {
        QGraphicsTextItem* ptiImgFmt=new QGraphicsTextItem(QObject::tr("Image Format: ")
                                                           +QString::number(mui->get_img_name_number_part_size())+ "."
                                                           +QString::number(mui->get_img_name_suffix_size()) );
        ptiImgFmt->setPos(pos().x() +iBlockSize*50 ,iLine);
        add_item(ptiImgFmt);

    }

    if(pM->get_show_colors())
        set_background_color(QColor(250,235,205));
    //        set_background_color(QColor(242,219,222));
}
////////////////////////////////////////////////////////////////////////////
