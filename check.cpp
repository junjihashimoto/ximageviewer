#include "image.h"
#include <cstring>
#include <cassert>
#include <iostream>

using namespace std;
int
check_data(const char* file_name){
  data file;
  if(data_load(file,file_name,24)<0)
    return -1;
  
  if(file.len>=2&&memcmp(file.head,"BM",2)==0)
    {data_free(file);return 0;}
  if(file.len>=2&&memcmp(file.head,"P1",2)==0)
    {data_free(file);return 0;}
  if(file.len>=2&&memcmp(file.head,"P2",2)==0)
    {data_free(file);return 0;}
  if(file.len>=2&&memcmp(file.head,"P3",2)==0)
    {data_free(file);return 0;}
  if(file.len>=2&&memcmp(file.head,"P4",2)==0)
    {data_free(file);return 0;}
  if(file.len>=2&&memcmp(file.head,"P5",2)==0)
    {data_free(file);return 0;}
  if(file.len>=2&&memcmp(file.head,"P6",2)==0)
    {data_free(file);return 0;}
#ifdef LIBPNG
  if(file.len>=4&&memcmp(file.head+1,"PNG",3)==0)
    {data_free(file);return 0;}
#endif
  if(file.len>=4&&memcmp(file.head,"GIF8",4)==0)
    {data_free(file);return 0;}
  if(file.len>=4&&memcmp(file.head,"hsi1",4)==0)
    {data_free(file);return 0;}
  if(file.len>=2&&memcmp(file.head,"\xff\xd8",2)==0){
    /*
    if(file.len>=10&&memcmp(file.head+6,"JFIF",4)==0)
      {data_free(file);return 0;}
    if(file.len>=10&&memcmp(file.head+6,"Exif",4)==0)
      {data_free(file);return 0;}
    if(file.len>=10&&memcmp(file.head+6,"Adob",4)==0)
      {data_free(file);return 0;}
    if(file.len>=10&&memcmp(file.head+6,"\0\0\x1\x5",4)==0)
      {data_free(file);return 0;}
    */
    data_free(file);return 0;
  }
  //assert(false);
//   cerr << "not suppored file format" << endl;
  //  exit(1);
  data_free(file);
  return -1;
}
int
data2image(const data& file,image& i){
  if(file.len>=2&&memcmp(file.head,"BM",2)==0)
    return bmp2image(file,i);
  if(file.len>=2&&memcmp(file.head,"P1",2)==0)
    return ppm2image(file,i);
  if(file.len>=2&&memcmp(file.head,"P2",2)==0)
    return ppm2image(file,i);
  if(file.len>=2&&memcmp(file.head,"P3",2)==0)
    return ppm2image(file,i);
  if(file.len>=2&&memcmp(file.head,"P4",2)==0)
    return ppm2image(file,i);
  if(file.len>=2&&memcmp(file.head,"P5",2)==0)
    return ppm2image(file,i);
  if(file.len>=2&&memcmp(file.head,"P6",2)==0)
    return ppm2image(file,i);
#ifdef LIBPNG
  if(file.len>=4&&memcmp(file.head+1,"PNG",3)==0)
    return png2image(file,i);
#endif
  if(file.len>=4&&memcmp(file.head,"GIF8",4)==0)
    return gif2image(file,i);
  if(file.len>=4&&memcmp(file.head,"hsi1",4)==0)
    return jpeg2image(file,i);
  if(file.len>=2&&memcmp(file.head,"\xff\xd8",2)==0){
    /*
    if(file.len>=10&&memcmp(file.head+6,"JFIF",4)==0)
      return jpeg2image(file,i);
    if(file.len>=10&&memcmp(file.head+6,"Exif",4)==0)
      return jpeg2image(file,i);
    if(file.len>=10&&memcmp(file.head+6,"Adob",4)==0)
      return jpeg2image(file,i);
    if(file.len>=10&&memcmp(file.head+6,"\0\0\x1\x5",4)==0)
      return jpeg2image(file,i);
    */
    return jpeg2image(file,i);
  }
  //assert(false);
  cerr << "not suppored file format" << endl;
  //  exit(1);
  return -1;
}
