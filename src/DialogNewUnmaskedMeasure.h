// this file is covered by the General Public License version 2 or later
// please see GPL.html for more details and licensing issues

// copyright Paul Crubille ( the author )
// mailto: paul@crubille.lautre.net

#ifndef NEWUNMASKED_H
#define NEWUNMASKED_H

#include <QDialog>
#include <QFileDialog>

#include <string>
#include <vector>
using namespace std;

#include "UnmaskedFoucaultImgSet.h"


class Mirror;

namespace Ui {
class DialogNewUnmaskedMeasure;
}

#define STATE_INIT 0
#define STATE_IMAGES_DEFINED 1
#define STATE_CROPPING 2
#define STATE_CROPPED 3
#define STATE_ANALYZING 4
#define STATE_ANALYZED 5


class DialogNewUnmaskedMeasure : public QDialog
{
  Q_OBJECT

public:
    explicit DialogNewUnmaskedMeasure(Mirror *pMirror, QWidget *parent = 0);
    ~DialogNewUnmaskedMeasure();

  void set_mirror_total_diameter( double diameter ); 
  double get_mirror_total_diameter(); 
  unsigned int get_when();
  float get_slit_angle(); 
  double get_mirror_angle(); 
  int    get_number_of_images();
  double get_delta_hx(); 
  int    get_name_format_n();
  int    get_name_format_s();

  UnmaskedFoucaultImgSet images_set;
  
private:
    Ui::DialogNewUnmaskedMeasure *ui;
    Mirror* _pMirror;

  QString _sFileName_calibration;
  QString _sFileName_first_image;


  int _STATE ; // From STATE_INIT
               //          -- (enter|browse image ... ) -> STATE_INIT
               //          -- /images_checked_ok/ -> STATE_IMAGES_DEFINED
               //          --  (Cancel button) -> !END return false
               // STATE_IMAGES_DEFINED 
               //          -- change (image name, browse, number_images, format, algo) -> STATE_INIT
               //          -- /cropped_button_checked_on/ -> STATE_CROPPED
               //          -- (cropped_button_clicked_on) -> STATE_CROPPED
               //          -- (crop_images) -> STATE_CROPPING
               //          --  (Cancel button) -> !END return false
               // STATE_CROPPING -- /complete/ -> STATE_CROPPED
               // STATE_CROPPED 
               //          -- change(image name, browse, number_images, format, algo) -> STATE_INIT
               //          -- (compute_null_zones) -> STATE_ANALYZING
               //          -- (cropped_button_clicked_off) -> STATE_INIT
               //          --  (Cancel button) -> !END return false
               // STATE_ANALYZING -- /complete/ -> STATE_ANALYZED
               // STATE_ANALYZED
               //          -- (generate_measure)/produce measure/ -> !END return true
               //          -- change(image name, browse, number_images, format, algo ) -> STATE_INIT
               //          -- change(slit_angle, RGB_channel) -> STATE_CROPPED
               //          --  (Cancel button) -> !END return false

  
  
  //  double _dSlit_angle;
  //  double _dMirror_angle;
  int _dNumber_of_images;
  double _dDeltaHx;
  int _dName_format_n;
  int _dName_format_s;


private slots:
    void on_pushButton_crop_clicked();
    void on_pushButton_compute_clicked();
    void on_pushButton_generate_measure_clicked();
    void on_pushButton_cancel_clicked();

  void on_buttonFileCalibration_clicked();
  void on_buttonFirstImg_clicked();


    void on_radioButton_Gray_clicked();
    void on_radioButton_R_clicked();
    void on_radioButton_G_clicked();
    void on_radioButton_B_clicked();

  void on_radioButton_cropped_clicked(); //already cropped images

  void on_le_nb_img_editingFinished();
  void on_le_d_hx_editingFinished();
  void on_leSlitAngle_editingFinished();

  void on_cbDrawImgType_activated( int index );
  void on_cbDrawImgSize_activated( int index );
  
  vector<double> get_Img_hx();

private:  
  bool cropped_button_on();
private:  
  vector<double> _Img_Hx;
  void enter_STATE_INIT();
  void resize_vectors();
  QString i_image_name( int i );
  void compute_hx(){}
};

#endif // NEWUNMASKED_H
