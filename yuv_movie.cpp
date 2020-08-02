extern "C"{

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
  
#include <unistd.h>
}
#include <cassert>
#include "data.h"
#include "image.h"

struct yuv_file{
  FD fd;
  data file;
};

static 
int
yuv_free(movie& mo){
  yuv_file& yuv=*(yuv_file*)mo.head;
  data_free(yuv.file);
  
  free(mo.head);
  mo.head=NULL;
  return 0;
}

static 
int
rgb_read(const movie& mo,int pos_frame,image& im){
  assert(0<=pos_frame&&pos_frame<mo.end_pos);
  yuv_file& yuv=*(yuv_file*)mo.head;
  int frame_len=3*mo.height*mo.width/2;

  lseek(yuv.fd,frame_len*pos_frame,SEEK_SET);
  get_data(yuv.fd,yuv.file,frame_len);
  itu_yuv2image(yuv.file,mo.height,mo.width,im);
  return 0;
}

static 
int
yuv_read(const movie& mo,int pos_frame,itu_yuv_image& im){
  assert(0<=pos_frame&&pos_frame<mo.end_pos);
  yuv_file& yuv=*(yuv_file*)mo.head;
  int frame_len=3*mo.height*mo.width/2;

  lseek(yuv.fd,frame_len*pos_frame,SEEK_SET);
  get_data(yuv.fd,yuv.file,frame_len);
  im.data=yuv.file.head;
  im.height=mo.height;
  im.width=mo.width;
  im.len=3*mo.height*mo.width/2;
  return 0;
}

int
yuv2movie(char* file_name,int height,int width,float bit_rate,
	  movie& mo){
  yuv_file* yuv=(yuv_file*)malloc(sizeof(yuv_file));
  assert(yuv!=NULL);
  
  yuv->fd=open(file_name,O_RDONLY);
  assert(yuv->fd>=0);

  struct stat s;
  fstat(yuv->fd,&s);
  
  mo.head=(char*)yuv;
  mo.pos=0;
  mo.end_pos=(s.st_size)/(3*height*width/2);
  mo.bit_rate=bit_rate;
  mo.height=height;
  mo.width=width;

  mo.read=rgb_read;
//   mo.write=yuv_write;
  mo.write=NULL;

  mo.yuv_read=yuv_read;
  mo.yuv_write=NULL;
  
  mo.free=yuv_free;
  return 0;
}

#ifdef YUVMOVIECPP_H
int
main(int argc,char** argv){
  movie mo;
  yuv2movie(argv[1],atoi(argv[2]),atoi(argv[3]),1,mo);

  image im;
  MOVIE_READ(mo,atoi(argv[4]),im);
  
  data file;
  image2bmp(im,file);
  data_save(file,"/dev/stdout");
  return 0;  
}
#endif //YUVMOVIECPP_H
