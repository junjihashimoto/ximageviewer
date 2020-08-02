#ifndef COLORCPP_MAIN
#define COLORCPP_MAIN
#include <cmath>
#include "image.h"

#define RGB_TRIM(v) ((int)((v)>255?255:((v)<0?0:(v))))
#define ITUY_TRIM(v) ((int)((v)>235?235:((v)<16?16:(v))))
#define ITUC_TRIM(v) ((int)((v)>240?240:((v)<16?16:(v))))


inline int
RGB2YIQ(const RGB& rgb,YIQ& yiq){
  yiq.y=0.2990*rgb.r+0.5870*rgb.g+0.1140*rgb.b;
  yiq.i=0.5959*rgb.r-0.2750*rgb.g-0.3210*rgb.b;
  yiq.q=0.2065*rgb.r-0.4969*rgb.g+0.2904*rgb.b;
  return 0;
}
inline int
YIQ2RGB(const YIQ& yiq,RGB& rgb){
  double dbuf;
  dbuf=yiq.y+0.9489*yiq.i+0.6561*yiq.q;
  if(dbuf>255)
    dbuf=255;
  else if(dbuf<0)
    dbuf=0;
  else
    dbuf=rint(dbuf);
  rgb.r=static_cast<unsigned char>(dbuf);
  dbuf=yiq.y-0.2645*yiq.i-0.6847*yiq.q;
  if(dbuf>255)
    dbuf=255;
  else if(dbuf<0)
    dbuf=0;
  else
    dbuf=rint(dbuf);
  rgb.g=static_cast<unsigned char>(dbuf);
  dbuf=yiq.y-1.1270*yiq.i+1.8050*yiq.q;
  if(dbuf>255)
    dbuf=255;
  else if(dbuf<0)
    dbuf=0;
  else
    dbuf=rint(dbuf);
  rgb.b=static_cast<unsigned char>(dbuf);
  return 0;
}



extern RGB (*itu_yuv_rgbtable)[256][256][256];
extern ITU_YUV (*rgb_itu_yuvtable)[256][256][256];
extern bool tablep;
extern int create_itu_yuv_and_rgbtable();


inline int
RGB2ITU_YUV(const RGB& rgb,ITU_YUV& yuv){
  int y,u,v;
#ifndef FAST
  y=lrint(0.257*rgb.r + 0.504*rgb.g + 0.098*rgb.b + 16.0);
  u=lrint(-0.148*rgb.r - 0.291*rgb.g + 0.439*rgb.b + 128.0);
  v=lrint(0.439*rgb.r - 0.368*rgb.g - 0.071*rgb.b + 128.0);
  yuv.y =ITUY_TRIM(y);
  yuv.u =ITUC_TRIM(u);
  yuv.v =ITUC_TRIM(v);
#else //FAST  
  if(!tablep)
    create_itu_yuv_and_rgbtable();
  yuv=(*rgb_itu_yuvtable)[rgb.r][rgb.g][rgb.b];
#endif //FAST    
  return 0;
}


inline int
ITU_YUV2RGB(const ITU_YUV& yuv,RGB& rgb){
  int r,g,b;
#ifndef FAST
  r =lrint(1.164*(yuv.y-16.0) -0.002*(yuv.u-128.0)  + 1.596*(yuv.v-128.0));
  g =lrint(1.164*(yuv.y-16.0) - 0.391*(yuv.u-128.0) - 0.813*(yuv.v-128.0));
  b =lrint(1.164*(yuv.y-16.0) + 2.018*(yuv.u-128.0) -0.001*(yuv.v-128.0));
  rgb.r=RGB_TRIM(r);
  rgb.g=RGB_TRIM(g);
  rgb.b=RGB_TRIM(b);
#else //FAST  

  if(!tablep)
    create_itu_yuv_and_rgbtable();
  rgb=(*itu_yuv_rgbtable)[yuv.y][yuv.u][yuv.v];
#endif //FAST
  return 0;
}
#endif //COLORCPP_MAIN
