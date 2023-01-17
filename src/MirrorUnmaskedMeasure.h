#ifndef _MirrorUnmaskedMeasure_
#define _MirrorUnmaskedMeasure_

#include "MirrorItem.h"
#include <string>
using namespace std;

#define WORK_TYPE_UNDEFINED 0
#define WORK_TYPE_ROUGH_GRINDING 1
#define WORK_TYPE_FINE_GRINDING 2
#define WORK_TYPE_POLISHING 3
#define WORK_TYPE_FIGURING 4

class MirrorUnmaskedMeasure : public MirrorItem
{
public:
  MirrorUnmaskedMeasure(){
    _sType="MirrorUnmaskedMeasure";
};

  double get_mirror_total_diameter(){ return mirror_total_diameter;};
  int get_RGB_channel(){ return RGB_channel ; };
  double get_slit_angle(){ return slit_angle;};
  string& get_flat_image_name(){ return flat_image_name;};
  string& get_first_image_name(){ return first_image_name;};
  int get_number_images(){ return number_images;};
  double get_delta_hx(){ return delta_hx;};
  int get_img_name_number_part_size(){ return img_name_number_part_size;};
  int get_img_name_suffix_size(){ return img_name_suffix_size;};

  void set_mirror_total_diameter( double value ){
    mirror_total_diameter = value ;};
  void set_RGB_channel( int value ){ RGB_channel = value ;};
  void set_slit_angle( double value ){ slit_angle = value ;};
  void set_flat_image_name( string value ){ flat_image_name = value ;};
  void set_first_image_name( string value ){ first_image_name = value ;};
  void set_number_images( int value ){ number_images = value ;};
  void set_delta_hx( double value ){ delta_hx = value ;};
  void set_img_name_number_part_size( int value ){
    img_name_number_part_size = value ;};
  void set_img_name_suffix_size( int value ){
    img_name_suffix_size = value ;};

private:
  double mirror_total_diameter;
  int RGB_channel;
  double slit_angle;
  string flat_image_name;
  string first_image_name;
  int number_images;
  double delta_hx;
  int img_name_number_part_size;
  int img_name_suffix_size;
  
};

#endif
