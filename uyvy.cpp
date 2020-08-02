#include "image.h"
#include <cassert>
#include <cstdlib>
int
uyvy2image(const data& file,image& i){
  int height=480;
  int width=720;
  image_init(i,height,width);
  assert(i.data!=NULL);
  CastArray<RGB*> d(i.data,i.width);
  for(int h=0;h<i.height;h++)
    for(int w=0;w<i.width;w++){
      ITU_YUV yuv;
      RGB rgb;
      yuv.y=file.head[(h*width+w)*2+1];
      yuv.u=file.head[(h*width+w)*2+((h*width+w)%2==0?0:-2)];
      yuv.v=file.head[(h*width+w)*2+((h*width+w)%2==1?0:2)];
      ITU_YUV2RGB(yuv,rgb);
      d[h][w]=rgb;
    }
  return 0;
}
