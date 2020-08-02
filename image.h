#ifndef IMAGECPP_H
#define IMAGECPP_H
#include <cstdlib>
#include "data.h"

extern "C"{
struct RGB{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char alpha;
};

struct rRGB{
  double r;
  double g;
  double b;
  double alpha;
};
}

struct image{
  RGB* data;
  int len;
  int width;
  int height;
  inline image():data(NULL),len(0),width(0),height(0){};
  inline RGB* operator [] (int i) const {return data+i*width;}
  inline operator RGB* () const {return data;}
};


template<class Type>
class CastArray{
  Type pointer;
  int size;
public:
  template<class OriType>
  inline CastArray(OriType p,int s):pointer(static_cast<OriType>(p)),size(s){}
  inline Type operator [] (int i) const {return pointer+i*size;}
  inline operator Type () const {return pointer;}
};

struct itu_yuv_image{//4:2:0
  char* data;
  int len;
  int width;
  int height;
  inline itu_yuv_image():data(NULL),len(0),width(0),height(0){};
};

struct rimage{
  rRGB* data;
  int len;
  int width;
  int height;
  inline rimage():data(NULL),len(0),width(0),height(0){};
  inline rRGB* operator [] (int i) const {return data+i*width;}
  inline operator rRGB* () const {return data;}
};
struct YIQ{
  double y;
  double i;
  double q;
  inline YIQ():y(0),i(0),q(0){};
};
struct ITU_YUV{
  unsigned char y;
  unsigned char u;
  unsigned char v;
  inline ITU_YUV():y(0),u(128),v(128){};
};

//image.cpp
extern int image_free(image& i);
extern int image_init(image& i,int height,int width);
extern int image_init(const image& oi,image& i);
extern int image_copy(const image& oi,image& i);
extern unsigned char ruchar(double v);
extern int itu_yuv_image_free(itu_yuv_image& i);



//bmp.cpp
extern int bmp2image(const data& file,image& i);
extern int image2bmp(const image& i,data& file,int bpp=24,char** bmp_into=NULL,char** pos_data=NULL);
extern int rawbmp2image(const data& file,int height,int width,image& i);

//ppm.cpp
extern int ppm2image(const data& file,image& i);
extern int image2ppm(const image& i,data& file);

//jpeg.cpp
extern int jpeg2image(const data& file,image& i);
extern int image2jpeg(const image& i,data& file);

//gif.cpp
extern int gif2image(const data& file,image& i);

//png.cpp
#define LIBPNG
#ifdef LIBPNG
extern int png2image(const data& file,image& i);
#endif

//yuv.cpp
extern int itu_yuv2image(const data& file,int height,int width,image& i);
extern int image2itu_yuv(const image& i,data& file);

//check.cpp
extern int check_data(const char* file_image);
extern int data2image(const data& file,image& i);



#include "color.h"

//rrgb.cpp
extern int rimage_init(const image& rgb,rimage& rrgb);
extern int image2rimage(const image& rgb,rimage& rrgb);
extern int rimage2image(const rimage& rrgb,image& rgb);



struct movie{
  char* head;
  int pos;
  int end_pos;
  float bit_rate;
  int height;
  int width;
  int (*read)(const movie& mo,int pos_frame,image& im);
  int (*write)(movie& mo,int pos_frame,const image& im);
  int (*yuv_read)(const movie& mo,int pos_frame,itu_yuv_image& im);
  int (*yuv_write)(movie& mo,int pos_frame,const itu_yuv_image& im);
  int (*free)(movie& mo);
  inline movie():
    head(NULL),
    pos(0),end_pos(0),
    bit_rate(1),
    height(0),width(0),
    read(NULL),write(NULL),
    free(NULL){}
};

#define MOVIE_READ(mo,pos,im) (mo).read(mo,pos,im)
#define MOVIE_WRITE(mo,pos,im) (mo).write(mo,pos,im)
#define MOVIE_YUV_READ(mo,pos,yuvim) (mo).yuv_read(mo,pos,yuvim)
#define MOVIE_YUV_WRITE(mo,pos,yuvim) (mo).yuv_write(mo,pos,yuvim)
#define MOVIE_FREE(mo) (mo).free(mo)

//movie.cpp
extern int movie_free(movie& mo);

//yuv_movie.cpp
extern int yuv2movie(char* file_name,int height,int width,float bit_rate,
		     movie& mo);
//uyvy2image.cpp
extern int uyvy2image(const data& file,image& i);
#endif //IMAGECPP_H
