#include "image.h"

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
