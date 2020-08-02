#include "image.h"
int
movie_free(movie& mo){
  MOVIE_FREE(mo);
  free(mo.head);
  mo.head=NULL;
  mo.pos=0;mo.end_pos=0;
  mo.bit_rate=1;
  mo.height=0;mo.width=0;
  mo.read=NULL;mo.write=NULL;mo.free=NULL;
  return 0;
}

