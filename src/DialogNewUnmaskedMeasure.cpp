// this file is covered by the General Public License version 2 or later
// please see GPL.html for more details and licensing issues

// copyright Paul Crubille ( the author )
// mailto: paul@crubille.lautre.net

#include "DialogNewUnmaskedMeasure.h"
#include "MirrorUnmaskedMeasure.h"
#include "MirrorCouderMeasure.h"
#include "Mirror.h"

#include <QProgressDialog>
#include <QMessageBox>
#include "ui_DialogNewUnmaskedMeasure.h"

#include <cassert>
using std::vector;

//////////////////////////////////////////////////////////////////////////////
DialogNewUnmaskedMeasure::DialogNewUnmaskedMeasure(Mirror *pMirror, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewUnmaskedMeasure)
{
    assert(pMirror);

    ui->setupUi(this);
    _pMirror=pMirror;
    //    ui->twHx->horizontalHeader()->setVisible(true);

    ui->dateTimeEdit->setDate(QDate::currentDate());
    ui->dateTimeEdit->setTime(QTime::currentTime());
    set_mirror_total_diameter( _pMirror->diameter() );
    //    _dMirror_angle =0.;
    _dNumber_of_images = ui->le_nb_img->text().toDouble();
    resize_vectors();
    _dDeltaHx =0.5;
    _dName_format_n =7;
    _dName_format_n =3;
    _STATE = STATE_INIT;
}
/////////////////////////////////////////////////////////////////////////////
DialogNewUnmaskedMeasure::~DialogNewUnmaskedMeasure()
{
    delete ui;
}
/////////////////////////////////////////////////////////////////////////////////
unsigned int DialogNewUnmaskedMeasure::get_when()
{
    return ui->dateTimeEdit->dateTime().toSecsSinceEpoch();
}
//////////////////////////////////////////////////////////////////////////////
float DialogNewUnmaskedMeasure::get_slit_angle()
{
    return ui->leSlitAngle->text().toFloat();
}
//////////////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::on_leSlitAngle_editingFinished()
{
    if( _STATE == STATE_INIT )
        return;
    _STATE = STATE_IMAGES_DEFINED;
}
//////////////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::set_mirror_total_diameter( double diameter )
{
    ui->leMirrorTotalDiameter->setText( QString::number( diameter) );
}
//////////////////////////////////////////////////////////////////////////////
double DialogNewUnmaskedMeasure::get_mirror_total_diameter()
{
    return ui->leMirrorTotalDiameter->text().toFloat();
}
//////////////////////////////////////////////////////////////////////////////
double DialogNewUnmaskedMeasure::get_mirror_angle()
{
    return ui->leMirrorAngle->text().toFloat();
}
//////////////////////////////////////////////////////////////////////////////
int DialogNewUnmaskedMeasure::get_number_of_images()
{
    return _dNumber_of_images;
}
//////////////////////////////////////////////////////////////////////////////
double DialogNewUnmaskedMeasure::get_delta_hx()
{
    return _dDeltaHx;
}
//////////////////////////////////////////////////////////////////////////////
int DialogNewUnmaskedMeasure::get_name_format_n()
{
    return ui->le_number_size->text().toDouble();
}
//////////////////////////////////////////////////////////////////////////////
int DialogNewUnmaskedMeasure::get_name_format_s()
{
    return ui->le_suffix_size->text().toDouble();
}
//////////////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::on_le_nb_img_editingFinished()
{
    int nb = ui->le_nb_img->text().toDouble();
    if( _dNumber_of_images != nb )
    {
        if( nb >= 3 )
        {
            _dNumber_of_images = nb;
            resize_vectors();
            enter_STATE_INIT();
        } else {
            QMessageBox::critical(this,tr("Error"),tr("Images number should be at least 3.)"));
            ui->le_nb_img->setText( QString::number( _dNumber_of_images) );
            //	  ui->le_nb_img->setFocus();
        }
    }
}
///////////////////////////////////////////////////////////////////////
QString DialogNewUnmaskedMeasure::i_image_name( int i )
{
    QString prefix;
    QString suffix;
    QString str_img_number;
    int img_number, l_suffixe, l_number;
    l_suffixe = ui->le_suffix_size->text().toDouble();
    l_number = ui->le_number_size->text().toDouble();
    suffix = ui->leFirsImgName->text().right( l_suffixe+1 ) ;
    prefix = ui->leFirsImgName->text(); prefix.chop( l_suffixe+1 ) ;
    str_img_number = prefix.right( l_number );
    prefix.chop( l_number ) ;
    img_number = i+ str_img_number.toInt();

    str_img_number.setNum( img_number );
    return prefix + str_img_number + suffix;
}
///////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::enter_STATE_INIT()
{
    images_set.set_rotation( get_slit_angle() );
    _STATE = STATE_INIT;
    for(int i = _dNumber_of_images; i <  MaxZones; i++ )
    {
        images_set.clear_image( i );
    };
    _STATE = STATE_IMAGES_DEFINED;
    images_set.show_images();
}
///////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::resize_vectors()
{
    float hxi;
    assert(_dNumber_of_images == ui->le_nb_img->text().toDouble());
    //unselect all items
    for(int i=0;i< MaxZones ;i++)
    {
        QTableWidgetItem* pItem=ui->twHx->item(0,i);
        if(pItem)
            pItem->setSelected(false);
    }

    for(int i=0;i< MaxZones ;i++)
    {
        if( i<_dNumber_of_images)
        {
            QString name_i = i_image_name( i );
            QTableWidgetItem* qti =new QTableWidgetItem("-");
            ui->twHx->setItem(0,i,qti);

            qti =new QTableWidgetItem("-");
            ui->twHx->setItem(1,i,qti);

            qti =new QTableWidgetItem("-");
            ui->twHx->setItem(2,i,qti);

            qti =new QTableWidgetItem( name_i );
            qti->setTextAlignment( Qt::AlignBottom );
            ui->twHx->setItem(3,i,qti);

            qti =new QTableWidgetItem("-");
            ui->twHx->setItem(4,i,qti);

            qti =new QTableWidgetItem("-");
            ui->twHx->setItem(5,i,qti);

            hxi = _dDeltaHx*i ;//  - _dDeltaHx*_dNumber_of_images ;
            qti =new QTableWidgetItem(QString::number( hxi,'f',2));
            ui->twHx->setItem(6,i,qti);
        }
        else
        {
            for(int l=0; l<7; l++)
            {
                QTableWidgetItem* qti =new QTableWidgetItem("");
                ui->twHx->setItem(l,i,qti);
            }
        }
    }
}
///////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::on_le_d_hx_editingFinished()
{
    double dhx;
    dhx = ui->le_d_hx->text().toDouble();
    if( _dDeltaHx != dhx )
    {
        _dDeltaHx = dhx;
        compute_hx();
        enter_STATE_INIT();
    }
    resize_vectors();
}    
//////////////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::on_pushButton_crop_clicked()
{
    //  QString img_filename;
    QTableWidgetItem* qti;
    bool file_exist;
    double x_center, y_center, radius;
    //
    QProgressDialog progress("Loading images...", "continue", 0, _dNumber_of_images, this);
    progress.setWindowModality(Qt::WindowModal);
    /*
    for (int i = 0; i < numFiles; i++) {

        if (progress.wasCanceled())
            break;
        //... copy one file
    }
    progress.setValue(numFiles);
  //
  */

    if(_STATE == STATE_IMAGES_DEFINED )
    {
        _STATE = STATE_CROPPING ;

        //  CHARGEMENT IMAGES
        for(int i=0;i< get_number_of_images() ;i++)
        {
            progress.setValue(i);
            file_exist = images_set.set_image( i, i_image_name( i ));
            if( ! file_exist )
            { // popup no file
                //fprintf(stderr, "No file %s\n", i_image_name( i ).toStdString().c_str());
                assert(false); //todo
            }

            if( images_set.foucaultsnapshot( i )->find_center_3points( get_slit_angle() ))
            {
                x_center = images_set.foucaultsnapshot( i )->center_x();
                y_center = images_set.foucaultsnapshot( i )->center_y();
                radius = images_set.foucaultsnapshot( i )->radius();

                qti =new QTableWidgetItem(QString::number( x_center,'f',2));
                ui->twHx->setItem(0,i,qti);

                qti =new QTableWidgetItem(QString::number( y_center,'f',2));
                ui->twHx->setItem(1,i,qti);

                qti =new QTableWidgetItem(QString::number( radius,'f',2));
                ui->twHx->setItem(2,i,qti);

            }
        }
        images_set.show_images();
        _STATE = STATE_CROPPED ;
    }
}
////////////////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::on_pushButton_compute_clicked()
{
    double total_diameter = get_mirror_total_diameter();
    //
    if( total_diameter <  _pMirror->diameter() )
    {
        total_diameter =  _pMirror->diameter() ;
        set_mirror_total_diameter( _pMirror->diameter() );
    }
    double total_edge_mask = ( total_diameter - _pMirror->diameter() )/2 +_pMirror->edge_mask_width();
    //fprintf( stderr, " total_diameter %f - optical_diameter %f ", total_diameter, _pMirror->diameter() );
    if( _STATE == STATE_CROPPED )
    {
        _STATE = STATE_ANALYZING ;
        QProgressDialog progress("Searching zones...", "continue", 0, _dNumber_of_images, this);
        progress.setWindowModality(Qt::WindowModal);
        /*
    for (int i = 0; i < numFiles; i++) {

        if (progress.wasCanceled())
            break;
        //... copy one file
    }
    progress.setValue(numFiles);
  //
  */
        for(int i=0;i< get_number_of_images() ;i++)
        {
            progress.setValue(i);
            if( images_set.foucaultsnapshot( i )->diff_180())
            {
                int nb_zones = images_set.foucaultsnapshot( i )->find_zones( _pMirror->hole_diameter()/ total_diameter, total_edge_mask / total_diameter );
                if( nb_zones >= 1 )
                {
                    QTableWidgetItem* qti =new QTableWidgetItem("-");
                    double zone1 = images_set.foucaultsnapshot( i )->InternalZone();
                    zone1 = zone1 * total_diameter / ( images_set.foucaultsnapshot( i )->diff_mirror.width() );
                    qti = new QTableWidgetItem(QString::number( zone1,'f',2));
                    ui->twHx->setItem(4,i,qti);
                    if( nb_zones >= 2 )
                    {
                        double zone2 = images_set.foucaultsnapshot( i )->ExternalZone();
                        zone2 = zone2 * total_diameter / ( images_set.foucaultsnapshot( i )->diff_mirror.width() );
                        qti =new QTableWidgetItem(QString::number( zone2,'f',2));
                        ui->twHx->setItem(5,i,qti);

                    }
                }
            }
        }
        // search for the null zone in each image
        _STATE = STATE_ANALYZED;

    }
    images_set.show_images();
}
////////////////////////////////////////////////////////////////
class hx_hm
{
public:
    double hx, hm;
    void set( double x, double m){ hx = x; hm = m;}
};
class less_than_hm
{
public:
    inline bool operator() (const hx_hm& a, const hx_hm& b)
    {
        return (a.hm < b.hm);
    }
};
///////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::on_pushButton_generate_measure_clicked()
{
    std::vector< hx_hm > v_hx_hm;

    if( _STATE == STATE_ANALYZED )
    {
        MirrorCouderMeasure* UnmaskedMeasure = new MirrorCouderMeasure(_pMirror);
        // VOIR DANS MainWindow.cpp on_actionNew_Couder_Measure_triggered

        //printf("generate measure - ");
        for(int i=0;i< MaxZones ;i++)
        {
            QTableWidgetItem* pItemHm=ui->twHx->item(6,i); //Hm
            if(pItemHm && (pItemHm->text().size()>0))
            {
                double Hx = pItemHm->text().toDouble();
                // Zone 1
                QTableWidgetItem* pItemZ1=ui->twHx->item(4,i); //Hm
                if(pItemZ1 && (pItemZ1->text().size()>0))
                {
                    double HmZ1 = pItemZ1->text().toDouble();
                    if( HmZ1 > 0 ) // OK we get a measure
                    {
                        hx_hm hxhm; hxhm.set( Hx, HmZ1 );
                        v_hx_hm.push_back( hxhm );
          //              fprintf(stderr, "image %d Hx= %f Hm=%f\n", i , Hx ,HmZ1 );
                    }
                }
                // Zone 2
                QTableWidgetItem* pItemZ2=ui->twHx->item(5,i); //Hm
                if(pItemZ2 && (pItemZ2->text().size()>0))
                {
                    double HmZ2 = pItemZ2->text().toDouble();
                    if( HmZ2 > 0 ) // OK we get a measure
                    {
                        hx_hm hxhm; hxhm.set( Hx, HmZ2 );
                        v_hx_hm.push_back( hxhm );
            //            fprintf(stderr, "image %d Hm= %f Hx=%f\n", i , Hx ,HmZ2 );
                    }
                }
            }
        }
        /*

            {
                pItem->setSelected(true);
                QMessageBox::critical(this,tr("Error"),tr("Please enter a valid Hx (i.e. a numerical value)"));
                return;
            }

      */

        std::sort(v_hx_hm.begin(), v_hx_hm.end(), less_than_hm());

        vector<double> vdHm;
        vector<double> vdHx;
        double Hm_old = 0.;
        for (auto it = v_hx_hm.begin() ; it != v_hx_hm.end(); ++it)
        {
            if( it->hm > Hm_old + 0.1 )
            {
                Hm_old = it->hm ;
                vdHm.push_back( it->hm );
                vdHx.push_back( it->hx );
         //       fprintf(stderr, "zone Hx= %f Hm=%f\n", it->hx, it->hm );
            }
            //else fprintf(stderr, "reject zone Hx= %f Hm=%f\n", it->hx, it->hm );
        };

        UnmaskedMeasure->set_measure(vdHx, vdHm, "Unmasked foucault") ;//nm.get_aspect());
        UnmaskedMeasure->set_measure_meridian( get_mirror_angle() );

        // Insert a MirrorUnmaskedMeasure in the item list:
        MirrorUnmaskedMeasure* Unmasked_Params = new MirrorUnmaskedMeasure();
        Unmasked_Params->set_when( get_when());
        Unmasked_Params->set_mirror_total_diameter( get_mirror_total_diameter() );

        Unmasked_Params->set_RGB_channel( images_set.get_RGB_channel() );
        Unmasked_Params->set_slit_angle( get_slit_angle() );
        Unmasked_Params->set_flat_image_name( ui->leCalImgName ->text().toStdString() );
        Unmasked_Params->set_first_image_name( ui->leFirsImgName->text().toStdString() );
        Unmasked_Params->set_number_images( get_number_of_images() );
        Unmasked_Params->set_delta_hx( get_delta_hx() );
        Unmasked_Params->set_img_name_number_part_size( get_name_format_n());
        Unmasked_Params->set_img_name_suffix_size( get_name_format_s());

        _pMirror->add_item( Unmasked_Params );
        // Insert the measure in the item list
        UnmaskedMeasure->set_when( get_when());
        _pMirror->add_item( UnmaskedMeasure );

        accept();
    }
}
///////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::on_pushButton_cancel_clicked()
{
//    printf("cancel - ");
    reject();
}
///////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::on_buttonFileCalibration_clicked()
{
    if((_STATE == STATE_CROPPING )||(_STATE == STATE_ANALYZING ))
    { // wait
        return ;}
    _sFileName_calibration = QFileDialog::getOpenFileName(this,tr("Open Foucault images"), ".", tr("Images Files (*.JPG)"));

    if (!_sFileName_calibration.toStdString().empty())
    {
        //        load_file(_sFileName);
        ui->leCalImgName->setText( _sFileName_calibration );
        enter_STATE_INIT();
    }
    //else     printf("file not found");
}
///////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::on_buttonFirstImg_clicked()
{
    if((_STATE == STATE_CROPPING )||(_STATE == STATE_ANALYZING ))
    { // wait
        return ;
    }
    _STATE = STATE_INIT;
    _sFileName_first_image = QFileDialog::getOpenFileName(this,tr("Open Foucault images"), ".", tr("Images Files (*.JPG)"));

    if (!_sFileName_first_image.toStdString().empty())
    {
        //        load_file(_sFileName);
        ui->leFirsImgName->setText( _sFileName_first_image );
        resize_vectors();
        enter_STATE_INIT();
    }
    //else     printf("file not found\n");
}
//////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::on_radioButton_Gray_clicked()
{
    images_set.set_RGB_channel( channel_Gray );
    resize_vectors();
    enter_STATE_INIT();
}
////////////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::on_radioButton_R_clicked()
{
    images_set.set_RGB_channel( channel_R );
    resize_vectors();
    enter_STATE_INIT();
}
////////////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::on_radioButton_G_clicked()
{
    images_set.set_RGB_channel( channel_G );
    resize_vectors();
    enter_STATE_INIT();
}
////////////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::on_radioButton_B_clicked()
{
    images_set.set_RGB_channel( channel_B );
    resize_vectors();
    enter_STATE_INIT();
}
////////////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::on_radioButton_cropped_clicked()
{
    if( cropped_button_on() )
    {
        if( _STATE == STATE_IMAGES_DEFINED)
        { // wait
            _STATE = STATE_CROPPED;
        }
    } else if(( _STATE == STATE_CROPPED) || ( _STATE == STATE_ANALYZED ) )
    { // wait
        enter_STATE_INIT();
    }
}
////////////////////////////////////////////////////////////////////////////
vector<double> DialogNewUnmaskedMeasure::get_Img_hx()
{ 
    return _Img_Hx;
}
//////////////////////////////////////////////////////////////////////
bool DialogNewUnmaskedMeasure::cropped_button_on()
{
    //    return ui->radioButton_cropped->isDown() ;
    return false;
}
//////////////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::on_cbDrawImgType_activated( int index )
{
    images_set.set_DrawImg_State( index );
    images_set.show_images();
}
//////////////////////////////////////////////////////////////////////////////
void DialogNewUnmaskedMeasure::on_cbDrawImgSize_activated( int index )
{
    images_set.set_DrawImg_Size( index );
    images_set.show_images();
}
//////////////////////////////////////////////////////////////////////////////
