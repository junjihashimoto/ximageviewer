#include <vector>
#include <algorithm>
using namespace std;
#include "yuv_util.h"
#include "data.h"



int
rgb_median(image& im){
  image imtmp;
  image_copy(im,imtmp);
  CastArray<RGB*> d(imtmp.data,imtmp.width);
  CastArray<RGB*> d2(im.data,im.width);
  vector<unsigned char> pixels(9);
  for(int h=1;h<im.height-1;h++){
    for(int w=1;w<im.width-1;w++){
      pixels[0]=d[h-1][w-1].r;
      pixels[1]=d[h-1][w].r;
      pixels[2]=d[h-1][w+1].r;
      pixels[3]=d[h][w-1].r;
      pixels[4]=d[h][w].r;
      pixels[5]=d[h][w+1].r;
      pixels[6]=d[h+1][w-1].r;
      pixels[7]=d[h+1][w].r;
      pixels[8]=d[h+1][w+1].r;
      sort(pixels.begin(),pixels.end());
      d2[h][w].r=pixels[4];

      pixels[0]=d[h-1][w-1].g;
      pixels[1]=d[h-1][w].g;
      pixels[2]=d[h-1][w+1].g;
      pixels[3]=d[h][w-1].g;
      pixels[4]=d[h][w].g;
      pixels[5]=d[h][w+1].g;
      pixels[6]=d[h+1][w-1].g;
      pixels[7]=d[h+1][w].g;
      pixels[8]=d[h+1][w+1].g;
      sort(pixels.begin(),pixels.end());
      d2[h][w].g=pixels[4];

      pixels[0]=d[h-1][w-1].b;
      pixels[1]=d[h-1][w].b;
      pixels[2]=d[h-1][w+1].b;
      pixels[3]=d[h][w-1].b;
      pixels[4]=d[h][w].b;
      pixels[5]=d[h][w+1].b;
      pixels[6]=d[h+1][w-1].b;
      pixels[7]=d[h+1][w].b;
      pixels[8]=d[h+1][w+1].b;
      sort(pixels.begin(),pixels.end());
      d2[h][w].b=pixels[4];
    }
  }
  return 0;
}

int
rgb_average(image& im){
  image imtmp;
  image_copy(im,imtmp);
  CastArray<RGB*> d(imtmp.data,imtmp.width);
  CastArray<RGB*> d2(im.data,im.width);
  for(int h=1;h<im.height-1;h++){
    for(int w=1;w<im.width-1;w++){
      d2[h][w].r=ruchar((1.0/11.0)*(
			  (double)d[h-1][w-1].r +d[h-1][w].r+d[h-1][w+1].r
			  +d[h][w-1].r  +3.0*d[h][w].r  +d[h][w+1].r
			  +d[h+1][w-1].r+d[h+1][w].r+d[h+1][w+1].r
			  ));
      d2[h][w].g=ruchar((1.0/11.0)*(
			  (double)d[h-1][w-1].g +d[h-1][w].g+d[h-1][w+1].g
			  +d[h][w-1].g  +3.0*d[h][w].g  +d[h][w+1].g
			  +d[h+1][w-1].g+d[h+1][w].g+d[h+1][w+1].g
			  ));
      d2[h][w].b=ruchar((1.0/11.0)*(
			  (double)d[h-1][w-1].b +d[h-1][w].b+d[h-1][w+1].b
			  +d[h][w-1].b  +3.0*d[h][w].b  +d[h][w+1].b
			  +d[h+1][w-1].b+d[h+1][w].b+d[h+1][w+1].b
			  ));
    }
  }
  return 0;
}


int
rgb_hist(image& im2){
  image im;
  image_copy(im2,im);
  int hist[256];
  int max=0;
  for(int i=0;i<256;i++)
    hist[i]=0;
  
  for(int i=0;i<im.len;i++){
    YIQ yiq;
    RGB2YIQ(im.data[i],yiq);
    hist[ruchar(yiq.y)]++;
    if(hist[ruchar(yiq.y)]>max)
      max=hist[ruchar(yiq.y)];
  }
  for(int i=0;i<im.height;i++)
    for(int j=0;j<im.width;j++){
      (im2.data+i*im.width+j)->r=(im2.data+i*im.width+j)->g=(im2.data+i*im.width+j)->b=0;
    }
  for(int j=0;j<im.width;j++){
    for(int i=0;i<im.height;i++){
      if((im.height-i)<(im.height/(double)max)*hist[(int)ruchar((256/(double)im.width)*j)])
	(im2.data+i*im.width+j)->r=(im2.data+i*im.width+j)->g=(im2.data+i*im.width+j)->b=255;
    }
  }
  image_free(im);
  return 0;
}

int
rgb_sharp(image& im){
  image imtmp;
  image_copy(im,imtmp);
  CastArray<RGB*> d(imtmp.data,imtmp.width);
  CastArray<RGB*> d2(im.data,im.width);
  for(int h=1;h<im.height-1;h++){
    for(int w=1;w<im.width-1;w++){
      double dt;
      dt=
	-d[h-1][w].r
	-d[h][w-1].r  +5*d[h][w].r  -d[h][w+1].r
	-d[h+1][w].r;
      ;
      d2[h][w].r=ruchar(dt);
      dt=
	-d[h-1][w].g
	-d[h][w-1].g  +5*d[h][w].g  -d[h][w+1].g
	-d[h+1][w].g;
      ;
      d2[h][w].g=ruchar(dt);
      dt=
	-d[h-1][w].b
	-d[h][w-1].b  +5*d[h][w].b  -d[h][w+1].b
	-d[h+1][w].b;
      ;
      d2[h][w].b=ruchar(dt);
    }
  }
  image_free(imtmp);
  return 0;
}

int
rgb_rap(image& im){
  image imtmp;
  image_copy(im,imtmp);
  CastArray<RGB*> d(imtmp.data,imtmp.width);
  CastArray<RGB*> d2(im.data,im.width);
  for(int h=1;h<im.height-1;h++){
    for(int w=1;w<im.width-1;w++){
      double dt;
      dt=
	+d[h-1][w].r
	+d[h][w-1].r  -4*d[h][w].r  +d[h][w+1].r
	+d[h+1][w].r;
      d2[h][w].r=ruchar(dt);
      dt=
	+d[h-1][w].g
	+d[h][w-1].g  -4*d[h][w].g  +d[h][w+1].g
	+d[h+1][w].g;
      d2[h][w].g=ruchar(dt);
      dt=
	+d[h-1][w].b
	+d[h][w-1].b  -4*d[h][w].b  +d[h][w+1].b
	+d[h+1][w].b;
      d2[h][w].b=ruchar(dt);
    }
  }
  image_free(imtmp);
  return 0;
}

int
rgb_resize(image& im,int w,int h){
  image im2;
  image_init(im2,h,w);
  CastArray<RGB*> d(im.data,im.width);
  CastArray<RGB*> d2(im2.data,im2.width);
  for(int i=0;i<im2.height;i++)
    for(int j=0;j<im2.width;j++)
      d2[i][j]=d[(im.height*i)/h][(im.width*j)/w];
  image_free(im);
  im=im2;
  return 0;
}
int
rgb_trim(image& im,int max_width,int max_height){
  if(im.width>max_width||im.height>max_height){
    if(im.width/im.height>max_width/max_height)
      rgb_resize(im,max_width,(im.height*max_width)/im.width);
    else
      rgb_resize(im,(im.width*max_height)/im.height,max_height);
  }
  return 0;
}
