#include "image.h"
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#define TRIMHEAD() \
while(1){ \
  assert(pos!=end); \
  if(*pos==' '||*pos=='\n') \
    pos++; \
  else \
    break; \
}
#define SETTAIL() \
while(1){ \
  assert(pos!=end); \
  if(*pos!=' '&&*pos!='\n') \
    pos++; \
  else{ \
    assert(pos!=end); \
    *pos++=0; \
    break; \
  } \
}


int
ppm2image(const data& file,image& im){
  int mode;
  data f;
  data_init(f);
  data_push(f,file.head,file.len);
  data_push(f,"\n",1);
  char *pos=f.head;
  char *end=f.head+f.len;
  
  assert(pos!=end);
  assert(*pos++=='P');

  assert(pos!=end);
  mode=*pos-'0';
  assert(1<=mode&&mode<=6);
  pos++;
  
  assert(pos!=end);
  assert(*pos++=='\n');

  char *head=NULL;
  im.height=0;
  im.width=0;
  int color=0;
  while(head==NULL){
    assert(pos!=end);
    if(*pos=='#'){
      pos++;
      while(1){
	assert(pos!=end);
	if(*pos++=='\n')
	  break;
      }
    }else{
      while(1){
	char* p=pos;
	while(1){
	  assert(pos!=end);
	  if(*pos==' '||*pos=='\n'){
	    *pos=0;
	    pos++;
	    break;
	  }
	  pos++;
	}
	if(im.width==0){
	  im.width=atoi(p);
	  assert(im.width!=0);
	}else if(im.height==0){
	  im.height=atoi(p);
	  assert(im.height!=0);
	  if(mode==1||mode==4){
	    head=pos;
	    break;
	  }
	}else{
	  color=atoi(p);
	  assert(color!=0);
	  head=pos;
	  break;
	}
      }
    }
  }
  im.len=im.width*im.height;
  assert(im.len!=0);
  im.data=(RGB*)malloc(im.len*sizeof(RGB));
  assert(im.data!=NULL);
  switch(mode){
  case 1:
    for(int i=0;i<im.len;i++){
      int v;
      TRIMHEAD();
      char* h=pos;
      SETTAIL();
      v=atoi(h);
      im.data[i].r=im.data[i].g=im.data[i].b=(v==0?0:255);
    }
    break;
  case 2:
    for(int i=0;i<im.len;i++){
      int v;
      TRIMHEAD();
      char* h=pos;
      SETTAIL();
      v=atoi(h);
      im.data[i].r=im.data[i].g=im.data[i].b=(v*255)/color;
    }
    break;
  case 3:
    for(int i=0;i<im.len*3;i++){
      int v;
      TRIMHEAD();
      char* h=pos;
      SETTAIL();
      v=atoi(h);
      switch(i%3){
      case 0:
	im.data[i/3].r=(v*255)/color;
	break;
      case 1:
	im.data[i/3].g=(v*255)/color;
	break;
      case 2:
	im.data[i/3].b=(v*255)/color;
	break;
      }
    }
    break;
  case 4:
    assert(im.len<=(end-head));
    for(int i=0;i<im.len;i++)
      im.data[i].r=im.data[i].g=im.data[i].b=(*pos++==0?0:255);
    break;
  case 5:
    assert(im.len<=(end-head));
    for(int i=0;i<im.len;i++)
      im.data[i].r=im.data[i].g=im.data[i].b=(*pos++*255)/color;
    break;
  case 6:
    assert(im.len*3<=(end-head));
    for(int i=0;i<im.len;i++){
      im.data[i].r=(*pos++*255)/color;
      im.data[i].g=(*pos++*255)/color;
      im.data[i].b=(*pos++*255)/color;
    }
    break;
  }
  data_free(f);
  return 0;
}
int
image2ppm(const image& i,data& file){
  data_init(file);
  data_push(file,"P6\n",3);
  char buf[1024];
  snprintf(buf,sizeof(buf),"%d %d\n",i.width,i.height);
  data_push(file,buf,strlen(buf));  
  data_push(file,"255\n",4);
  for(int j=0;j<i.len;j++){
    data_push(file,(char*)&(i.data[j].r),1);
    data_push(file,(char*)&(i.data[j].g),1);
    data_push(file,(char*)&(i.data[j].b),1);
  }
  return 0;
}
#ifdef PPMCPP_MAIN
int
main(){
  data file;
  image im;
  data_load(file,"/dev/stdin");
  ppm2image(file,im);
  data_free(file);
  RGB (*dbuf)[im.width];
  dbuf=(RGB (*)[im.width])(im.data);
  for(int i=0;i<im.height;i++){
    for(int j=0;j<im.width;j++){
      dbuf[i][j].r=255-dbuf[i][j].r;
      dbuf[i][j].g=255-dbuf[i][j].g;
      dbuf[i][j].b=255-dbuf[i][j].b;
    }
  }
  //  std::reverse(i.data.begin(),i.data.end());
  image2ppm(im,file);
  data_save(file,"/dev/stdout");
  return 0;
}

#endif

