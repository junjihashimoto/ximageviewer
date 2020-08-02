#include "image.h"
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>

int
itu_yuv2image(const data& file,int height,int width,image& i){
  if(file.len!=3*height*width/2)
    return -1;
  if(i.len!=height*width)
    image_init(i,height,width);

  CastArray<RGB*> d(i.data,i.width);

  CastArray<unsigned char *> y((unsigned char*)file.head,width);
  CastArray<unsigned char *> u((unsigned char*)file.head+height*width,width/2);
  CastArray<unsigned char *> v((unsigned char*)file.head+height*width+(height*width)/4,width/2);
  for(int h=0;h<height;h++)
    for(int w=0;w<width;w++){
      ITU_YUV yuv;
      yuv.y=y[h][w];yuv.u=u[h/2][w/2];yuv.v=v[h/2][w/2];
      RGB rgb;
      ITU_YUV2RGB(yuv,rgb);
      d[h][w]=rgb;
    }
  return 0;
}
int
image2itu_yuv(const image& i,data& file){
  typedef unsigned int uint;
  //  assert(false);
  data_init(file,i.len*3/2);
  CastArray<RGB*> d(i.data,i.width);
  CastArray<unsigned char *> y((unsigned char*)file.head,i.width);
  CastArray<unsigned char *> u((unsigned char*)file.head+i.height*i.width,i.width/2);
  CastArray<unsigned char *> v((unsigned char*)file.head+i.height*i.width+(i.height*i.width)/4,i.width/2);
  for(int h=0;h<i.height/2;h++)
    for(int w=0;w<i.width/2;w++){
      ITU_YUV yuv[4];
      RGB2ITU_YUV(d[h*2][w*2],yuv[0]);
      RGB2ITU_YUV(d[h*2][w*2+1],yuv[1]);
      RGB2ITU_YUV(d[h*2+1][w*2],yuv[2]);
      RGB2ITU_YUV(d[h*2+1][w*2+1],yuv[3]);
      y[h*2][w*2]=yuv[0].y;
      y[h*2][w*2+1]=yuv[1].y;
      y[h*2+1][w*2]=yuv[2].y;
      y[h*2+1][w*2+1]=yuv[3].y;
      u[h][w]=(uint)(((uint)yuv[0].u+(uint)yuv[1].u+(uint)yuv[2].u+(uint)yuv[3].u)/4);
      v[h][w]=(uint)(((uint)yuv[0].v+(uint)yuv[1].v+(uint)yuv[2].v+(uint)yuv[3].v)/4);
      /*
      u[h][w]=0;
      v[h][w]=0;
      */
  }

  
//   char buf[1024];
//   snprintf(buf,sizeof(buf),"%d %d\n",i.width,i.height);
//   data_push(file,buf,strlen(buf));  
//   data_push(file,"255\n",4);
//   for(int j=0;j<i.len;j++){
//     data_push(file,(char*)&(i.data[j].r),1);
//     data_push(file,(char*)&(i.data[j].g),1);
//     data_push(file,(char*)&(i.data[j].b),1);
//   }
  return 0;
}
#ifdef YUVCPP_MAIN
int
main(int argc,char** argv){
  if(argc!=3)
    return 1;
  data file;
  image im;
  data_load(file,"/dev/stdin");
  itu_yuv2image(file,atoi(argv[1]),atoi(argv[2]),im);
  data_free(file);
  image2bmp(im,file);
  data_save(file,"/dev/stdout");
  return 0;
}

#endif

