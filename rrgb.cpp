#include "image.h"
#include <cassert>
#include <cstdlib>
int
rimage_init(const image& rgb,rimage& rrgb){
  assert(rrgb.data==NULL);
  rrgb.len=rgb.len;
  rrgb.width=rgb.width;
  rrgb.height=rgb.height;
  rrgb.data=(rRGB*)malloc(rrgb.len*sizeof(rRGB));
  assert(rrgb.data!=NULL);
  return 0;
}
int
image2rimage(const image& rgb,rimage& rrgb){
  assert(rrgb.len==rgb.len);
  for(int i=0;i<rgb.len;i++){
    rrgb.data[i].r=rgb.data[i].r;
    rrgb.data[i].g=rgb.data[i].g;
    rrgb.data[i].b=rgb.data[i].b;
    rrgb.data[i].alpha=rgb.data[i].alpha;
  }
  return 0;
}
int
rimage2image(const rimage& rrgb,image& rgb){
  assert(rrgb.len==rgb.len);
  for(int i=0;i<rgb.len;i++){
    rgb.data[i].r=ruchar(rrgb.data[i].r);
    rgb.data[i].g=ruchar(rrgb.data[i].g);
    rgb.data[i].b=ruchar(rrgb.data[i].b);
    rgb.data[i].alpha=ruchar(rrgb.data[i].alpha);
  }
  return 0;
}
