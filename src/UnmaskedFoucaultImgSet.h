// this file is covered by the General Public License version 2 or later
// please see GPL.html for more details and licensing issues

// copyright Paul Crubille ( the author )
// mailto: paul@crubille.lautre.net

#ifndef _UnmaskedFoucaultImgSet_
#define _UnmaskedFoucaultImgSet_

#include <QDialog>
#include <QLabel>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QScrollArea>
#include <string>
#include <vector>

#include "Mirror.h"
#include "MirrorItem.h"
using namespace std;

#define MaxZones 10

#define channel_Gray 0
#define channel_R 1
#define channel_G 2
#define channel_B 3

#define NB_step_in_IMG 400

class FoucaultSnapshot
{
public:
  int image_number;
  QString file_name;
  QImage raw_image, cropped, diff_mirror;
  QPixmap pixmap;

  double min_pixel, max_pixel; // computed setting img to gray - used for normalisation computing diff..

private:
  double c_x_circle, c_y_circle, r_circle;
  double internal_zone_index, external_zone_index; int number_of_zones;
  double internal_zone_value, external_zone_value;

  double x_lower_edge, y_lower_edge, x_upper_edge, y_upper_edge;
  double find_edge_y( int x, int x_diff, int y_diff );
  
public:
  FoucaultSnapshot();
  bool find_center_3points(double angle);
  //  bool find_center_edge( double *x, double *y, double *radius);
  bool diff_180();
  int find_zones( double obstruction , double edge); // return number of zones (0, 1 or 2). obstruction and edge as ratio.
public:
  void draw_image( );
  void draw_cropped( );
  void draw_diff_180(  );
  int nb_zones() { return number_of_zones;}
  double InternalZone() { return internal_zone_index ;}
  double ExternalZone() { return external_zone_index ;}
  double center_x(){ return c_x_circle; } // in the original image
  double center_y(){ return c_y_circle; } // in the original image
  double radius(){ return r_circle ;}   // in the original image
  bool clear();
};

class UnmaskedFoucaultImgSet //: public QDialog
{
public:
  UnmaskedFoucaultImgSet();
  ~UnmaskedFoucaultImgSet();
  void set_rotation( float slit ){ Slit = slit; };
  void set_RGB_channel( int channel ); 
  int get_RGB_channel(){ return RGB_channel ;}; 
  bool clear_image( int i );
  bool set_image( int i, QString name);
  bool cropped_images();
  bool use_center_by3points(){return true;};
  bool use_center_by3edges(){return false;};
  bool use_center_by_callibration_image(){return false;};

  string& CalibrationImgName();

  FoucaultSnapshot *foucaultsnapshot( int i);
  void show_images();
  void set_DrawImg_State( int index );
  void set_DrawImg_Size( int index );
private:
  int DrawImgState;
  string _sComment;

  int NbCharNumImg;
  int NbImgs;
  float ImgStep;
  int RGB_channel;
public:
  FoucaultSnapshot foucault_img[ MaxZones ];
  float Slit; // to rotate images before analyse
  QLabel myLabel; // to draw imagespu
  QHBoxLayout *hlayout;
  QScrollArea *scrollarea;
  //    QWidget *imgs_window;
  QWidget *hbox_widget; //
public:
  int ChannelPixel(QRgb pixel )
{
  if( RGB_channel == channel_R ) return qRed( pixel ) ;
  if( RGB_channel == channel_G ) return qGreen( pixel ) ;
  if( RGB_channel == channel_B ) return qBlue( pixel ) ;
  if( RGB_channel == channel_Gray )
    {
      return  qGray( pixel );
      //  int+P = qRed( pixel ) + qGreen( pixel ) + qBlue( pixel ) ;
      //      return P/3 ;
    };
  return -1;
}

};

#endif
