#include "color.h"

RGB (*itu_yuv_rgbtable)[256][256][256];
ITU_YUV (*rgb_itu_yuvtable)[256][256][256];
bool tablep=false;

int
create_itu_yuv_and_rgbtable(){
  ITU_YUV yuv;
  RGB rgb;
  long int  y=0,u=0,v=0;
  long int  r=0,g=0,b=0;
  itu_yuv_rgbtable=(RGB (*)[256][256][256])malloc(sizeof(RGB)*256*256*256);
  rgb_itu_yuvtable=(ITU_YUV (*)[256][256][256])malloc(sizeof(ITU_YUV)*256*256*256);
  
  
//   r =lrint(1.164*(yuv.y-16)                    + 1.596*(yuv.v-128));
//   g =lrint(1.164*(yuv.y-16) - 0.391*(yuv.u-128) - 0.813*(yuv.v-128));
//   b =lrint(1.164*(yuv.y-16) + 2.018*(yuv.u-128));
//   rgb.r=RGB_TRIM(r);
//   rgb.g=RGB_TRIM(g);
//   rgb.b=RGB_TRIM(b);

//   y=lrint(0.257*rgb.r + 0.504*rgb.g + 0.098*rgb.b + 16);
//   u=lrint(-0.148*rgb.r - 0.291*rgb.g + 0.439*rgb.b + 128);
//   v=lrint(0.439*rgb.r - 0.368*rgb.g - 0.071*rgb.b + 128);
//   yuv.y =ITUY_TRIM(y);
//   yuv.u =ITUC_TRIM(u);
//   yuv.v =ITUC_TRIM(v);

  for(int y=0;y<256;y++)
    for(int u=0;u<256;u++)
      for(int v=0;v<256;v++){
	r =lrint(1.164*(ITUY_TRIM(y)-16)
		 + 1.596*(ITUY_TRIM(v)-128));
	g =lrint(1.164*(ITUY_TRIM(y)-16)
		 - 0.391*(ITUY_TRIM(u)-128)
		 - 0.813*(ITUY_TRIM(v)-128));
	b =lrint(1.164*(ITUY_TRIM(y)-16)
		 + 2.018*(ITUY_TRIM(u)-128));
	rgb.r=RGB_TRIM(r);
	rgb.g=RGB_TRIM(g);
	rgb.b=RGB_TRIM(b);
	(*itu_yuv_rgbtable)[y][u][v]=rgb;
      }
  for(int r=0;r<256;r++)
    for(int g=0;g<256;g++)
      for(int b=0;b<256;b++){
	y=lrint( 0.257*r + 0.504*g + 0.098*b + 16);
	u=lrint(-0.148*r - 0.291*g + 0.439*b + 128);
	v=lrint( 0.439*r - 0.368*g - 0.071*b + 128);
	yuv.y =ITUY_TRIM(y);
	yuv.u =ITUC_TRIM(u);
	yuv.v =ITUC_TRIM(v);
	(*rgb_itu_yuvtable)[r][g][b]=yuv;
      }
  tablep=true;  
  return 0;
}
