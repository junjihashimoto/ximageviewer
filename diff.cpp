#include "image.h"
#include "diff.h"
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <iostream>
using namespace std;


int
yuv_diff_pix(const itu_yuv_image& ori1,const itu_yuv_image& ori2,image& im,
	     unsigned int scale){
  unsigned int width=ori1.width;
  unsigned int height=ori1.height;
  if(ori1.width!=im.width||ori1.height!=im.height)
    image_init(im,ori1.height,ori1.width);
  CastArray<unsigned char *> y1((unsigned char*)ori1.data,width);
  CastArray<unsigned char *> u1((unsigned char*)ori1.data+height*width,width/2);
  CastArray<unsigned char *> v1((unsigned char*)ori1.data+height*width+(height*width)/4,width/2);
  CastArray<unsigned char *> y2((unsigned char*)ori2.data,width);
  CastArray<unsigned char *> u2((unsigned char*)ori2.data+height*width,width/2);
  CastArray<unsigned char *> v2((unsigned char*)ori2.data+height*width+(height*width)/4,width/2);
  
  CastArray<RGB *> d(im.data,im.width);

  for(unsigned int h=0;h<height;h++){
    for(unsigned int w=0;w<width;w++){
      d[h][w].r=RGB_TRIM(255-scale*abs((y1[h][w]-(int)y2[h][w])));
      d[h][w].g=RGB_TRIM(255-scale*abs((u1[h/2][w/2]-(int)u2[h/2][w/2])));
      d[h][w].b=RGB_TRIM(255-scale*abs((v1[h/2][w/2]-(int)v2[h/2][w/2])));
    }
  }
  return 0;
}



int
yuv_diff_pix(const image& ori1,const image& ori2,image& im,
	     unsigned int scale){
  if(ori1.width!=im.width||ori1.height!=im.height)
    image_init(im,ori1.height,ori1.width);
  ITU_YUV yuv1,yuv2,yuv;
  for(unsigned int i=0;i<(unsigned int)ori1.len;i++){
    RGB2ITU_YUV(ori1.data[i],yuv1);
    RGB2ITU_YUV(ori2.data[i],yuv2);
    im.data[i].r=RGB_TRIM(255-scale*abs((yuv2.y-yuv1.y)));
    im.data[i].g=RGB_TRIM(255-scale*abs((yuv2.u-(int)yuv1.u)));
    im.data[i].b=RGB_TRIM(255-scale*abs((yuv2.v-(int)yuv1.v)));
  }
  return 0;
}
int
yuv_diff_macro(const image& ori1,const image& ori2,image& diff_im,
	       unsigned int scale){
  if(ori1.width!=diff_im.width||ori1.height!=diff_im.height)
    image_init(diff_im,ori1.height,ori1.width);
  ITU_YUV yuv1,yuv2,yuv;
  CastArray<RGB*> a1(ori1.data,ori1.width),
                  a2(ori2.data,ori2.width),
                  ao(diff_im.data,diff_im.width);
  for(int i=0;i<ori1.height/16;i++)
    for(int j=0;j<ori1.width/16;j++){
      RGB ave;
      int ave_r=0,ave_g=0,ave_b=0;
      ITU_YUV yuv;
      for(int s=0;s<16;s++)
	for(int t=0;t<16;t++){
	  ave_r+=a2[i*16+s][j*16+t].r-a1[i*16+s][j*16+t].r;
	  ave_g+=a2[i*16+s][j*16+t].g-a1[i*16+s][j*16+t].g;
	  ave_b+=a2[i*16+s][j*16+t].b-a1[i*16+s][j*16+t].b;
	}
      ave_r=ave_r/(16*16*2);
      ave_g=ave_g/(16*16*2);
      ave_b=ave_b/(16*16*2);

      ave.r=255-RGB_TRIM(scale*abs(0.257*ave_r + 0.504*ave_g + 0.098*ave_b));
      ave.g=255-RGB_TRIM(scale*abs(-0.148*ave_r - 0.291*ave_g + 0.439*ave_b));
      ave.b=255-RGB_TRIM(scale*abs(0.439*ave_r - 0.368*ave_g - 0.071*ave_b ));
      
      for(int s=0;s<16;s++)
	for(int t=0;t<16;t++)
	  ao[i*16+s][j*16+t]=ave;
    }

  return 0;
}

int
yuv_diff_block(const image& ori1,const image& ori2,image& diff_im,
	       unsigned int scale){
  if(ori1.width!=diff_im.width||ori1.height!=diff_im.height)
    image_init(diff_im,ori1.height,ori1.width);
  ITU_YUV yuv1,yuv2,yuv;
  CastArray<RGB*> a1(ori1.data,ori1.width),
                  a2(ori2.data,ori2.width),
                  ao(diff_im.data,diff_im.width);
  for(int i=0;i<ori1.height/8;i++)
    for(int j=0;j<ori1.width/8;j++){
      RGB ave;
      int ave_r=0,ave_g=0,ave_b=0;
      ITU_YUV yuv;
      for(int s=0;s<8;s++)
	for(int t=0;t<8;t++){
	  ave_r+=a2[i*8+s][j*8+t].r-a1[i*8+s][j*8+t].r;
	  ave_g+=a2[i*8+s][j*8+t].g-a1[i*8+s][j*8+t].g;
	  ave_b+=a2[i*8+s][j*8+t].b-a1[i*8+s][j*8+t].b;
	}
      ave_r=ave_r/(8*8*2);
      ave_g=ave_g/(8*8*2);
      ave_b=ave_b/(8*8*2);

      ave.r=255-RGB_TRIM(scale*abs(0.257*ave_r + 0.504*ave_g + 0.098*ave_b));
      ave.g=255-RGB_TRIM(scale*abs(-0.148*ave_r - 0.291*ave_g + 0.439*ave_b));
      ave.b=255-RGB_TRIM(scale*abs(0.439*ave_r - 0.368*ave_g - 0.071*ave_b ));
      
      for(int s=0;s<8;s++)
	for(int t=0;t<8;t++)
	  ao[i*8+s][j*8+t]=ave;
    }

  return 0;
}

int
rgb_gradation(const vector<RGB>& color_table,image& im){
  int div=256/color_table.size();
  for(int i=0;i<im.len;i++)
    im.data[i]=color_table[im.data[i].r/div];
  return 0;
}


int
rgb_r_gray(image& im){
  for(int i=0;i<im.len;i++)
    im.data[i].g=im.data[i].b=im.data[i].r;
  return 0;
}
int
rgb_g_gray(image& im){
  for(int i=0;i<im.len;i++)
    im.data[i].r=im.data[i].b=im.data[i].g;
  return 0;
}
int
rgb_b_gray(image& im){
  for(int i=0;i<im.len;i++)
    im.data[i].r=im.data[i].g=im.data[i].b;
  return 0;
}

int
rgb_max_gray(image& im){
  for(int i=0;i<im.len;i++)
    im.data[i].r=im.data[i].g=im.data[i].b
      =min(im.data[i].r,min(im.data[i].g,im.data[i].b));
  return 0;
}

int
yuv_y_gray(image& im){
  ITU_YUV yuv;
  for(int i=0;i<im.len;i++){
    RGB2ITU_YUV(im.data[i],yuv);
    yuv.u=yuv.v=128;
    ITU_YUV2RGB(yuv,im.data[i]);
//     im.data[i].r=im.data[i].g=im.data[i].b=
//       ITUY_TRIM(lrint(((yuv.y-16.0)/(235.0-15.0))*256.0));
  }
  return 0;
}
int
yuv_u_gray(image& im){
  ITU_YUV yuv;
  for(int i=0;i<im.len;i++){
    RGB2ITU_YUV(im.data[i],yuv);
    yuv.y=255-yuv.u;
    yuv.u=yuv.v=128;
    ITU_YUV2RGB(yuv,im.data[i]);
  }
  return 0;
}
int
yuv_v_gray(image& im){
  ITU_YUV yuv;
  for(int i=0;i<im.len;i++){
    RGB2ITU_YUV(im.data[i],yuv);
    im.data[i].r=im.data[i].g=im.data[i].b=
      ITUC_TRIM(lrint(((yuv.v-16.0)/(240.0-15.0))*256.0));
  }
  return 0;
}

int
rgb_threshold(image& im,int min,int max){
  for(int i=0;i<im.len;i++){
    if(min<=im.data[i].r&&im.data[i].r<=max)
      im.data[i].r=im.data[i].g=im.data[i].b=255;
    else
      im.data[i].r=im.data[i].g=im.data[i].b=0;
  }
  return 0;
}

int
rgb_reverse(image& im){
  for(int i=0;i<im.len;i++){
    im.data[i].r=255-im.data[i].r;
    im.data[i].g=255-im.data[i].g;
    im.data[i].b=255-im.data[i].b;
  }
  return 0;
}
int
rgb_and(const image& mask,image& im){
  for(int i=0;i<im.len;i++){
    im.data[i].r&=mask.data[i].r;
    im.data[i].g&=mask.data[i].g;
    im.data[i].b&=mask.data[i].b;
  }
  return 0;
}



int
yuv_sort_diff_pix(const image& ori1,const image& ori2,
		  std::vector<int>& diff_y,
		  std::vector<int>& diff_u,
		  std::vector<int>& diff_v
		  ){
  ITU_YUV yuv1,yuv2,yuv;
  for(int i=0;i<ori1.len;i++){
    RGB2ITU_YUV(ori1.data[i],yuv1);
    RGB2ITU_YUV(ori2.data[i],yuv2);
    diff_y[i]=abs((yuv2.y-yuv1.y)/2);
    diff_u[i]=abs((yuv2.u-yuv1.u)/2);
    diff_v[i]=abs((yuv2.v-yuv1.v)/2);
  }
  sort(diff_y.begin(),diff_y.end());
  sort(diff_u.begin(),diff_u.end());
  sort(diff_v.begin(),diff_v.end());
  return 0;
}
int
yuv_sort_diff_macro(const image& ori1,const image& ori2,
		    std::vector<int>& diff_y,
		    std::vector<int>& diff_u,
		    std::vector<int>& diff_v
		    ){
  ITU_YUV yuv1,yuv2,yuv;
  CastArray<RGB*> a1(ori1.data,ori1.width),
                  a2(ori2.data,ori2.width);
  for(int i=0;i<ori1.height/16;i++)
    for(int j=0;j<ori1.width/16;j++){
      int ave_r=0,ave_g=0,ave_b=0;
      int ave_y=0,ave_u=0,ave_v=0;
      for(int s=0;s<16;s++)
	for(int t=0;t<16;t++){
	  ave_r+=a2[i*16+s][j*16+t].r-a1[i*16+s][j*16+t].r;
	  ave_g+=a2[i*16+s][j*16+t].g-a1[i*16+s][j*16+t].g;
	  ave_b+=a2[i*16+s][j*16+t].b-a1[i*16+s][j*16+t].b;
	}
      ave_r=ave_r/(16*16*2);
      ave_g=ave_g/(16*16*2);
      ave_b=ave_b/(16*16*2);

      ave_y=abs(lrint(0.257*ave_r + 0.504*ave_g + 0.098*ave_b));
      ave_u=abs(lrint(-0.148*ave_r - 0.291*ave_g + 0.439*ave_b));
      ave_v=abs(lrint(0.439*ave_r - 0.368*ave_g - 0.071*ave_b));
      for(int s=0;s<16;s++)
	for(int t=0;t<16;t++){
	  diff_y[i]=ave_y;
	  diff_u[i]=ave_u;
	  diff_v[i]=ave_v;
	}
    }
  sort(diff_y.begin(),diff_y.end());
  sort(diff_u.begin(),diff_u.end());
  sort(diff_v.begin(),diff_v.end());
  return 0;
}


#ifdef DIFFCPP_MAIN
int
main(int argc,char** argv){
  data file;
  image im1,im2,im;
  
  data_load(file,argv[1]);
  bmp2image(file,im1);
  data_free(file);
  
  data_load(file,argv[2]);
  bmp2image(file,im2);
  data_free(file);

  image_init(im1,im);

  
  yuv_diff_macro(im1,im2,im,atoi(argv[3]));
  
//   yuv_u_gray(im2);
//   rgb_threshold(im2,128,255);
//   //  rgb_reverse(im2);
//   rgb_and(im2,im1);
  image2bmp(im,file);
  data_save(file,"/dev/stdout");
  return 0;
}
#endif //DIFFCPP_MAIN
