// this file is covered by the General Public License version 2 or later
// please see GPL.html for more details and licensing issues

// copyright Paul Crubille ( the author )
// mailto: paul@crubille.lautre.net

#include "Mirror.h"
#include "UnmaskedFoucaultImgSet.h"
#include <math.h>

#include <QGuiApplication>
#include <QScreen>
#include <QPainter>

int Vignette_Size = 240;

FoucaultSnapshot::FoucaultSnapshot()
{}

void FoucaultSnapshot::draw_image( )
{
    if ( raw_image.isNull())
    { QPixmap nullqimage;
        pixmap = nullqimage;
    }
    else
    {
        double ratio = (double)raw_image.height() / (double)Vignette_Size;
        pixmap = pixmap.fromImage( raw_image );
        pixmap = pixmap.scaledToHeight( Vignette_Size, Qt::SmoothTransformation);
        QPainter painter( &pixmap );
        painter.setPen( QColor(0,255,0) );
        painter.setFont( QFont("Arial") );
        painter.drawText( QPoint(10, pixmap.height() - 20), file_name.right(12) );
        painter.drawEllipse( QPointF( c_x_circle/ratio, c_y_circle/ratio),
                             r_circle/ratio,
                             r_circle/ratio) ;
    };
}

void FoucaultSnapshot::draw_cropped()
{
    if ( cropped.isNull())
    {
        QPixmap nullqimage;
        pixmap = nullqimage;
    }
    else
    {
        double ratio = (double)cropped.height() / (double)Vignette_Size;
        pixmap = pixmap.fromImage( cropped );
        pixmap = pixmap.scaledToWidth( Vignette_Size, Qt::SmoothTransformation);
        QPainter painter( &pixmap );
        painter.setPen( QColor(0,255,0) );
        painter.setFont( QFont("Arial") );
        painter.drawText( QPoint(10, pixmap.height() - 20), file_name.right(12) );
        if( number_of_zones >= 1 )
        {
            painter.drawEllipse( QPointF( pixmap.height()/2, pixmap.width()/2),
                                 internal_zone_index/ratio,
                                 internal_zone_index/ratio) ;
            if( number_of_zones >= 2 )
            {
                painter.drawEllipse( QPointF( pixmap.height()/2, pixmap.width()/2),
                                     external_zone_index/ratio,
                                     external_zone_index/ratio) ;
            }
        }
    }
}

void FoucaultSnapshot::draw_diff_180()
{
    if ( diff_mirror.isNull())
    {
        QPixmap nullqimage;
        pixmap = nullqimage;
    }
    else
    {
        double ratio = (double)diff_mirror.height() / (double)Vignette_Size;
        pixmap = pixmap.fromImage( diff_mirror );
        pixmap = pixmap.scaledToWidth( Vignette_Size, Qt::SmoothTransformation);
        QPainter painter( &pixmap );
        painter.setPen( QColor(0,255,0) );
        painter.setFont( QFont("Arial") );
        painter.drawText( QPoint(10, pixmap.height() - 20), file_name.right(12) );
        if( number_of_zones >= 1 )
        {
            painter.drawEllipse( QPointF( pixmap.height()/2, pixmap.width()/2),
                                 internal_zone_index/ratio,
                                 internal_zone_index/ratio) ;
            if( number_of_zones >= 2 )
            {
                painter.drawEllipse( QPointF( pixmap.height()/2, pixmap.width()/2),
                                     external_zone_index/ratio,
                                     external_zone_index/ratio) ;
            }
        }
    }
}

UnmaskedFoucaultImgSet::UnmaskedFoucaultImgSet()
{
    Slit = 0;
    RGB_channel = channel_Gray;
    DrawImgState = 0;
    //  imgs_window = new QWidget;
    //  scrollarea = new QScrollArea( imgs_window );

    scrollarea = new QScrollArea ;  // ( this ); //QDialog
    scrollarea->setGeometry (QRect(0, 0, 1000, Vignette_Size));
    scrollarea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    scrollarea->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    scrollarea->setWidgetResizable( false );

    hbox_widget = new QWidget;
    hbox_widget->setGeometry (QRect(0, 0, 1000, Vignette_Size));
    scrollarea->setWidget( hbox_widget );

    hlayout = new QHBoxLayout( hbox_widget );
    hlayout->setGeometry (QRect(0, 0, 1000, Vignette_Size));
}

UnmaskedFoucaultImgSet::~UnmaskedFoucaultImgSet()
{
    scrollarea->close();
}

void UnmaskedFoucaultImgSet::set_RGB_channel( int channel ) 
{
    if( RGB_channel == channel )
        return ; // nothing to do

    for(int i=0; i< MaxZones ; i++)
    {
        clear_image( i );
        //      set_image(i , "" );
    }
    RGB_channel = channel;
}

void UnmaskedFoucaultImgSet::set_DrawImg_State( int index )
{
    DrawImgState = index;
}

void UnmaskedFoucaultImgSet::set_DrawImg_Size( int index )
{
    if( index == 0 ) Vignette_Size = 240;
    if( index == 1 ) Vignette_Size = 360;
    if( index == 2 ) Vignette_Size = 480;
    if( index == 3 ) Vignette_Size = 720;
    if( index >= 4 ) Vignette_Size = 1024;
}

void UnmaskedFoucaultImgSet::show_images()
{
    int hbox_size = 20;
    QLabel *ql;

    if( DrawImgState == 0 )
    {
        scrollarea->close();
    }
    else {

        QScreen *screen = QGuiApplication::primaryScreen();
        QRect  screenGeometry = screen->geometry();
        int screen_height = screenGeometry.height();
        int screen_width = screenGeometry.width();


        hbox_widget = new QWidget;
        hbox_widget->setGeometry (QRect(0, 0, screen_width, Vignette_Size));
        scrollarea->setWidget( hbox_widget );
        scrollarea->setGeometry (QRect(0, 0, screen_width, Vignette_Size));

        hlayout = new QHBoxLayout( hbox_widget );

        for(int i=0; i < MaxZones ; i++)
        {
            if( DrawImgState == 1 )
            {
                foucault_img[i].draw_image( );
            }
            if( DrawImgState == 2 )
            {
                foucault_img[i].draw_cropped( );
            }
            if( DrawImgState == 3 )
            {
                foucault_img[i].draw_diff_180( );
            }
            if( !foucault_img[i].pixmap.isNull() )
            {
                ql = new QLabel ;
                ql->setPixmap( foucault_img[i].pixmap );
                hlayout->addWidget( ql );
                hbox_size += foucault_img[i].pixmap.width() +10;
            }
        }

        hbox_widget->setGeometry (QRect(0, 0, hbox_size, Vignette_Size));
        hlayout->setGeometry (QRect(0, 0, hbox_size, Vignette_Size));
        scrollarea->takeWidget();
        scrollarea->setWidget( hbox_widget );

        scrollarea->move(0, screen_height - Vignette_Size -50);
        scrollarea->setWindowModality( Qt::WindowModal);
        scrollarea->show();
        //  this->show();
        scrollarea->setEnabled( true );
        scrollarea->activateWindow();
    }
}

bool UnmaskedFoucaultImgSet::set_image( int i, QString name)
{
    FILE *F; static QImage nullqimage;
    static QPixmap nullqpixmap;

    if(i >= MaxZones) return false;
    //    foucault_img[i].raw_image = nullqimage;
    foucault_img[i].cropped = nullqimage; foucault_img[i].diff_mirror = nullqimage; foucault_img[i].pixmap = nullqpixmap;

    if( name == foucault_img[i].file_name )
    {
        F = fopen( name.toStdString().c_str() , "r" );
        if( F != NULL )
        {
            fclose( F );
            foucault_img[i].draw_image( );
            return true;
        } else return false;
    } else
    {
        foucault_img[i].file_name = name;

        F = fopen( name.toStdString().c_str() , "r" );
        if( F != NULL )
        {
            QImage ImgIn;
            fclose( F );
            ImgIn.load( name );
            QImage GrayImg( ImgIn.width(), ImgIn.height(), QImage::Format_Grayscale8);
            /*
        if( RGB_channel == channel_Gray )
          {
        foucault_img[i].min_pixel = 0; foucault_img[i].max_pixel = 256;
        GrayImg = ImgIn.convertToFormat( QImage::Format_Grayscale8 );
          } else
        */
            {
                foucault_img[i].min_pixel = 256; foucault_img[i].max_pixel = 0;
                for (int y = 0; y < ImgIn.height(); ++y) {
                    for (int x = 0; x < ImgIn.width(); ++x) {
                        int pixvalue = ChannelPixel( ImgIn.pixel(x, y));
                        GrayImg.setPixel(x, y, QColor(pixvalue, pixvalue, pixvalue).rgb());
                        if( pixvalue < foucault_img[i].min_pixel )
                            foucault_img[i].min_pixel = pixvalue ;
                        if( pixvalue > foucault_img[i].max_pixel )
                            foucault_img[i].max_pixel = pixvalue ;

                    }
                }
            }
            foucault_img[i].raw_image = GrayImg;
        }
        foucault_img[i].draw_image( );
        return true;
    }
    return false;
}

bool FoucaultSnapshot::clear()
{
    static QImage nullqimage;static QPixmap nullqpixmap;
    bool R;
    R = !raw_image.isNull();
    raw_image = nullqimage; cropped = nullqimage; diff_mirror = nullqimage; pixmap = nullqpixmap;
    file_name = "" ;
    r_circle = 0 ;
    number_of_zones = 0 ;
    return R;
}
bool UnmaskedFoucaultImgSet::clear_image( int i )
{
    if(i >= MaxZones)
        return false;
    return foucault_img[i].clear() ;
}

FoucaultSnapshot *UnmaskedFoucaultImgSet::foucaultsnapshot( int i)
{
    return &foucault_img[ i ];
}

/////////////////////////////////////////////////////////////
//           ALGO TO FIND THE CENTER:
// 1 -  ROTATE THE IMAGE (slit)
// 2 -  FIND CENTER
// 3 -  CROP
//
// FIND CENTER is the main part:
// we use a specialised edge detection that is detection of the edge of a
// foucault image: the background is very dark, the left edge is very bright
// and it is at least half a circle.
// it should be very accurate so:
// 1 - detect the top and bottom edges.
//            The circle radius is at least y_top - y_bottom
// 2 - forget 1/8 of the top  and 1//8 of the bottom (not very accurate part
// 3 - consider the following of the image in three parts
// and enough ( 100 ) line in each. find the edge for each of theses lines
//   - compute the center and radius of the circle getting one point from each of theses parts
//   - forget the (center, radius) with smmaller min, max x, y and radius.
//   - average (x, y, radius).
class Circle
{
private:
    double X, Y, Radius ;
public:
    double x()
    {return X;}

    double y()
    {return Y;}

    double radius()
    {return Radius;}

    void thee_point_circle( double p1_x, double p1_y, double p2_x, double p2_y, double p3_x, double p3_y )
    {
        double A, B, C, D;

        A = p1_x - p2_x; B = p2_x - p3_x; C = p1_y - p2_y, D = p2_y - p3_y;
        // compute bisectrice intersection
        X = A*D*(p1_x+p2_x) - B*C*(p2_x+p3_x) + C*D*(p1_y+p2_y) - C*D*(p2_y+p3_y);
        X = X / (2.*(A*D-B*C));
        Y = B*(p2_x+p3_x) +D*(p2_y+p3_y) -2.*B*X;
        Y = Y /(2.*D);
        Radius = sqrt( pow(X- p1_x , 2 ) + pow( Y- p1_y ,2) );
    }
};

bool comp_radius( Circle c1, Circle c2 ){ return c1.radius() < c2.radius() ;}
bool comp_x( Circle c1, Circle c2 ){ return c1.x() < c2.x() ;}

bool FoucaultSnapshot::find_center_3points(double angle)
{
    double Y_step, img_height;
    double delta_raw_c_x_circle =0; // delta between raw and rotated image center
    double delta_raw_c_y_circle =0; // used to draw circle on vignette.
    int y;
    if( !raw_image.isNull())
    {
        QTransform tr;
        tr.rotate( angle );
        cropped = raw_image.transformed(tr, Qt::SmoothTransformation );
        cropped = cropped.convertToFormat( QImage::Format_Grayscale8 );
        delta_raw_c_x_circle = ( cropped.width() - raw_image.width() )/2. ;
        delta_raw_c_y_circle = ( cropped.height() - raw_image.height() )/2. ;

        img_height = cropped.height();
        Y_step = img_height / NB_step_in_IMG;
        // find the upper and lower point of the circle
        // give a first radius approximation.

        ////////////////////////////////////////////////////////////////
        // <begin> find lower edge (starting from y=0 to
        y = 2; // as find_edge( *, *, -1 ) look at pixel from y-2 and y+2
        while( find_edge_y( y, 0, 1 ) <=0 ) // search black to white from bottom.
        {
            y = y+ Y_step;
            if( y >= img_height-3 )
                return false; // no foucault mirror image
        };

        if( y < Y_step +2 )
        { y = 2;}
        else
        {
            y = y - Y_step ;
        }

        // restart edge search but for all x.
        while( (x_lower_edge = find_edge_y( y, 0, 1 )) <=0 ) // search black to white from bottom.
        {
            if( y >= img_height-3 )
                return false; // should not happend
            y = y+1;
        };

        y_lower_edge = y;
        // (x_lower_edge,y_lower_edge) is  the lower edge for the disk!
        // <end> find lower edge (starting from y=0 to

        ////////////////////////////////////////////////////////////////
        // <begin> find upper edge (starting from y=img_height-2 to
        y = img_height-3; // as find_edge( *, *, -1 ) look at pixel from y-2 and y+2
        while( find_edge_y( y, 0, -1 ) <=0 ) // search black to white from bottom.
        {
            y = y- Y_step;
            if( y <= 2 )
                return false; // no foucault mirror image
        };
        if( y > img_height -3 - Y_step )
        {
            y = img_height - 3;
        }
        else
        {
            y = y + Y_step ;
        }

        // restart edge search but for all x.
        while( (x_upper_edge = find_edge_y( y, 0, -1 )) <=0 ) // search black to white from bottom.
        {
            if( y <= 2)
                return false; // should not happend
            y = y-1;
        };

        y_upper_edge = y;
        // (x_upper_edge,y_upper_edge) is  the upper edge for the disk!
        // <end> find low edge (starting from y=0 to

        // to test first approximation
        c_y_circle = ( y_upper_edge + y_lower_edge )/2;
        r_circle =  (y_upper_edge - y_lower_edge)/2 ;
        int edge_x = x_upper_edge;
        for( int c_y = c_y_circle - 2* Y_step; c_y < c_y_circle + 2* Y_step; c_y++)
        {
            int new_edge_x = find_edge_y( c_y , 1, 0 );
            if( new_edge_x < edge_x) edge_x = new_edge_x;
        };

        c_x_circle = edge_x + r_circle ;
        int edge_60 = y_lower_edge - 2 + 2*r_circle/3;
        ////////////////////////////////////////////////////////////////
        // Now,find ( (edge_low - hedge_ height)/step )/3 triple points on the circle
        std::vector<Circle> vector_circle;
        for(int p_i_y = y_lower_edge; p_i_y < edge_60 ; p_i_y = p_i_y +Y_step )
        {
            float p1_i_x, p2_i_x, p3_i_x;
            float p2_i_y = p_i_y + (2*r_circle)/3. ;
            float p3_i_y = p_i_y + (4*r_circle)/3. ;
            p1_i_x = find_edge_y( p_i_y , 1, 1 );
            p2_i_x = find_edge_y( p2_i_y , 1, 0 );
            p3_i_x = find_edge_y( p3_i_y , 1, -1 );
            // compute center of the 3 points circle
            if( ( p1_i_x > 0 ) && ( p2_i_x > 0 ) && ( p3_i_x > 0 ) )
            {
                Circle circle_i;
                circle_i.thee_point_circle( p1_i_x, p_i_y, p2_i_x, p2_i_y, p3_i_x, p3_i_y );
                vector_circle.push_back( circle_i );
            }
        }
        /*
      // filtering: forget smallest and largest values for radius, x and y
      // then compute average.
      for(int i=0; i< 5; i++) forget_smaller_and_largest_radius_x_y();
      compute_average();
      */
        int nb_circle = (int)vector_circle.size();
        int nb_filtered = 0;
        c_x_circle = 0; c_y_circle = 0; r_circle = 0;
        if( nb_circle <= 8 )
            return false ; // very poor image

        std::sort( vector_circle.begin(), vector_circle.end(), comp_x );
        std::sort( vector_circle.begin() + nb_circle*4/16, vector_circle.begin() + nb_circle*14/16 , comp_radius );
        for (auto it=vector_circle.begin()+nb_circle*7/16; it!=vector_circle.begin()+nb_circle*12/16; ++it)
        {
            nb_filtered++ ;
            c_x_circle += it->x();
            c_y_circle += it->y();
            r_circle += it->radius();
        }
        c_x_circle = c_x_circle / nb_filtered ;
        c_y_circle = c_y_circle / nb_filtered ;
        r_circle = r_circle / nb_filtered ;

        cropped = cropped.copy( c_x_circle - r_circle, c_y_circle - r_circle, 2*r_circle, 2*r_circle );
        cropped = cropped.convertToFormat( QImage::Format_Grayscale8 );
        draw_cropped( );
        {
            // c_x_circle and c_y_circle are in the rotated and enlarged image (the image named "cropped" before copy)
            // "-angle " rotate around raw_image center
            double X, Y;
            double C_X = raw_image.width()/2. ;
            double C_Y = raw_image.height()/2. ;
            double rad_angle = angle * PI / 180. ;
            // first translate coords so they are relative to the center in an image of the size of the original image.
            X = c_x_circle - delta_raw_c_x_circle - C_X;
            Y = c_y_circle - delta_raw_c_y_circle - C_Y;
            // then rotate
            c_x_circle = C_X+  X * cos( rad_angle ) + Y * sin( rad_angle );
            c_y_circle = C_Y+  (-X) * sin( rad_angle ) + Y * cos( rad_angle );
        }
        return true;
    }
    else
        return false;
}

double FoucaultSnapshot::find_edge_y( int y, int x_diff, int y_diff )
{
    double pixvalue, pixvalue_m1, pixvalue_m2, pixvalue_p1, pixvalue_p2 ;
    for( int i=abs(2*x_diff)+10; i< ( cropped.width() -2 - abs(2*x_diff)  ); i++ )
    {
        pixvalue = qGray( cropped.pixel(i, y) );
        pixvalue_m1 = qGray( cropped.pixel(i - x_diff, y - y_diff ));
        pixvalue_p1 = qGray( cropped.pixel(i + x_diff, y + y_diff ));
        pixvalue_m2 = qGray( cropped.pixel(i - 2*x_diff, y - 2*y_diff ));
        pixvalue_p2 = qGray( cropped.pixel(i + 2*x_diff, y + 2*y_diff ));

        int k_edge = (max_pixel - min_pixel)/32; //rather good but can be a user selectable param
        if(    ( pixvalue >= min_pixel + (max_pixel - min_pixel)*1./2. )
               && ( pixvalue_p2 > pixvalue_m2 +4*k_edge ) && ( pixvalue_p1 > pixvalue_m2 +3*k_edge)
               && ( pixvalue -k_edge > pixvalue_m1 ) && ( pixvalue -2*k_edge > pixvalue_m2 )
               )
        {
            return i;
        }
    }
    return -1;
}

/////////////////////////////////////////////////////////////
//           ALGO TO FIND THE ZONES:
// 1 -  COMPUTE diff_mirror
// 2 -  FIND zones
// 3 -  set zones
bool FoucaultSnapshot::diff_180()
{
    if( !cropped.isNull() )
    {
        QImage GrayImg( cropped.width(), cropped.height(), QImage::Format_Grayscale8);
        double normalize = 1;
        if( 255 > (max_pixel - min_pixel) ) normalize = 255./(max_pixel - min_pixel);
        int H = cropped.height(); int W = cropped.width();
        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                double pixvalue = normalize * fabs( qGray( cropped.pixel(x, y) ) -
                                                    qGray( cropped.pixel( W-x-1, H-y-1) ) );
                GrayImg.setPixel(x, y, QColor(pixvalue, pixvalue, pixvalue).rgb());
            }
        }
        diff_mirror = GrayImg;
    }
    else return false;
    draw_diff_180();
    return true;
}

class table_trigo
{
    double *table_cos, *table_sin;
public:
    table_trigo( double angle, int sample )
    {
        double r_sample;
        table_cos = new double[ sample ];
        table_sin = new double[ sample ];
        r_sample = sample; angle = angle*3.14/180;
        for(int i = 0 ; i < sample; i = i+1)
        {
            table_cos[ i ] = cos( (double)i * angle/r_sample ) ;
            table_sin[ i ] = sin( (double)i * angle/r_sample ) ;
        }
    }

    ~table_trigo()
    {
        delete table_cos;
        delete table_sin;
    }

    double cos_scaled( int index ) // return cos( index * angle/sample ) as stored in table_cos
    {
        return table_cos[ index ];
    }
    double sin_scaled( int index ) // return cos( index * angle/sample ) as stored in table_cos
    {
        return table_sin[ index ];
    }
};

//
#define ANGLE_ZONE 20.
#define SAMPLE_ZONE_WIDTH 200
int FoucaultSnapshot::find_zones( double obstruction, double edge )
{
    table_trigo trigo( ANGLE_ZONE,  SAMPLE_ZONE_WIDTH );
    int x_index;
    //  int value_array[ diff_mirror.width()/2 ]; // core dump ... some time after the function ends WHY???
    static int value_array[ 10000 ]; // very large as primay images are at least 30 000 * 20 000
    double center = diff_mirror.width()/2. ;

    for(x_index = 0 ; x_index < center; x_index++ )
    {
        static int Arc[ 2*SAMPLE_ZONE_WIDTH ];
        value_array[ x_index ] = 0;
        for( int alpha = 0 ; alpha < SAMPLE_ZONE_WIDTH ; alpha++)
        {
            Arc[ alpha ]      = qGray( diff_mirror.pixel(
                                           center - x_index*trigo.cos_scaled( alpha ) ,
                                           center - x_index*trigo.sin_scaled( alpha )
                                           ));
            Arc[ alpha+SAMPLE_ZONE_WIDTH ] = qGray( diff_mirror.pixel(
                                                        center - x_index*trigo.cos_scaled( alpha ) ,
                                                        center + x_index*trigo.sin_scaled( alpha )
                                                        ));
        };
        // filtering : forget half the sample, the smaller and the bigger.
        // so there is still SAMPLE_ZONE_WIDTH. rather efficient for dust.
        std::vector<int> SortArc (Arc, Arc+2*SAMPLE_ZONE_WIDTH);
        std::sort (SortArc.begin(), SortArc.end());
        value_array[ x_index ] = 0;
        int SAMPLE_BEGIN = SAMPLE_ZONE_WIDTH/2; int SAMPLE_END = 6*SAMPLE_ZONE_WIDTH/4;
        for (auto it=SortArc.begin()+SAMPLE_BEGIN; it!=SortArc.begin()+SAMPLE_END; ++it)
            //      for( int alpha = 0 ; alpha < SAMPLE_ZONE_WIDTH; alpha++)
        {
            value_array[ x_index ] += *it;
        }
    }

    ////////////////////////////////////////////////////////////////
    // First, remember we adress conical mirrors and observe change in curvature radius.
    // To adress this, Couder screen zone size are tiny on the edge while the first zone is large.
    // The sizes are close to some constant divide by log_2 of the zones number.
    // We can use something close to smooth the curve: From value_array, for each index,
    // average +/- log_2 ( diff_mirror.width() )/log_2( index )
    // - So, computing integers, from a 1024 radius image, we get 10, 5, 3, 2, 1.
    // - So we forget too the log_2( img_size ) internals pixels and the last external pixel
    // (as we are unable to compute their averaged values).

    vector<int> average_array(diff_mirror.width()/2 );
    int log_2_img_width ; log_2_img_width = log2( diff_mirror.width()/2. );
    int search_begin, search_end;
    if( obstruction <= 0 )
    {
        search_begin = diff_mirror.width()*5./100. + log_2_img_width+1 ; // really, it miss some value (log2( search_begin), but it does not matter.
    } else
    {
        search_begin = obstruction*r_circle + log_2_img_width+1 ; // really, it miss some value (log2( search_begin), but it does not matter.
    };
    if( edge <= 0 )
    {
        search_end = diff_mirror.width()/2. -2 ;
    } else
    {
        search_end = r_circle - edge*r_circle -1 ;
    };
    for( int i = search_begin ; i< search_end; i++)
    {
        int log2_i =  1 + log_2_img_width - log2( i );
        average_array[ i ] = 0;
        for( int j = i - log2_i; j <= i + log2_i ; j++)
        {
            average_array[ i ] += value_array[ j ];
        }
        average_array[ i ] = average_array[ i ] / (2*log2_i +1 );
    }

    // Now, search for local minima in value_array.
    while( (search_begin < search_end) && ( average_array[ search_begin +1 ]  > average_array[ search_begin ] ))
        search_begin++;
    internal_zone_value = 65000 ;
    for( int index = search_begin ; index < search_end; index++)
    {
        if( average_array[ index ] < internal_zone_value )
        {
            int index_constant = index;
            internal_zone_value = average_array[ index ] ;
            // if there is an exactly constant zone:
            for( ; internal_zone_value == average_array[ index+1 ] ; index++ );
            internal_zone_index = ( index_constant+index )/2 ;
        }
    }

    // compute excluded zone
    int min_exclude, max_exclude;
    max_exclude = internal_zone_index + (1+ log_2_img_width - log2( internal_zone_index ))*diff_mirror.width()/100 ;
    min_exclude =  internal_zone_index - (1+ log_2_img_width - log2( internal_zone_index ))*diff_mirror.width()/100 ;
    while( (min_exclude > search_begin) && ( average_array[ min_exclude -1 ]  > average_array[ min_exclude ] ))
        min_exclude--;
    while( (max_exclude < search_end) && ( average_array[ max_exclude +1 ]  > average_array[ max_exclude ] ))
        max_exclude++;
    // Now, search for second local minima in value_array.
    external_zone_value = 65000 ;
    for( int index = search_begin ; index < search_end; index++)
    {
        if( (index < min_exclude ) || (index > max_exclude) )
        {
            if( average_array[ index ] < external_zone_value )
            {
                int index_constant = index;
                external_zone_value = average_array[ index  ] ;
                // if there is an exactly constant zone:
                for( ; external_zone_value == average_array[ index+1 ] ; index++ );
                external_zone_index = ( index_constant+index )/2 ;
            }
        }
    }
    if( internal_zone_value > (SAMPLE_ZONE_WIDTH*max_pixel)/16)
    {
        number_of_zones = 0 ;
    }
    else if(( external_zone_value > (SAMPLE_ZONE_WIDTH*max_pixel)/16 ) )
    {
        number_of_zones = 1 ;
    } else {
        number_of_zones = 2;
    };
    return number_of_zones;
}
