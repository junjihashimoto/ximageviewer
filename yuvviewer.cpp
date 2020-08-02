#include "image.h"

extern "C"{
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
}

#include <cstdio>
#include <map>
#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;


static int image_width;
static int image_height;

static int
resize(const image& im,image& im2,int w,int h){
  image_init(im2,h,w);
  
//   RGB (*dbuf1)[im.width];
//   dbuf1=(RGB (*)[im.width])(im.data);
//   RGB (*dbuf2)[im2.width];
//   dbuf2=(RGB (*)[im2.width])(im2.data);
  double rx=im.width/static_cast<double>(w),ry=im.height/static_cast<double>(h);
  for(int i=0;i<im2.height;i++)
    for(int j=0;j<im2.width;j++)
      *(im2.data+im2.width*sizeof(RGB)*i+j)
	=*(im.data+im.width*sizeof(RGB)*static_cast<int>(ry*i)+
	   static_cast<int>(rx*j));
  return 0;
}

static int
ShowImage(Display *d,Window w,image& im){
  XImage *img;
  int depth,bpp;
  char* data=NULL;
//    int x,y;
//    unsigned int wid,h,bw,dep;
//    XGetGeometry(d,RootWindow(d,0),&x,&y,&wid,&h,&bw,&dep);
  XResizeWindow(d,w,im.width,im.height);
  switch(depth=DefaultDepth(d,0)){
  case 16:
    bpp=2;
    data=(char*)malloc(bpp*im.width*im.height);
    if(ImageByteOrder(d)==MSBFirst){
      for(int i=0;i<im.len;i++){
	unsigned char r=im.data[i].r,g=im.data[i].g,b=im.data[i].b;
	*(data+i*2)=(r&0xf8)|(g>>5);
	*(data+i*2+1)=((g&0x1c)<<3)|(b>>3);
      }
    }else{
      for(int i=0;i<im.len;i++){
	unsigned char r=im.data[i].r,g=im.data[i].g,b=im.data[i].b;
	*(data+i*2)=((g&0x1c)<<3)|(b>>3);
	*(data+i*2+1)=(r&0xf8)|(g>>5);
      }
    }
    break;
  case 24:
    bpp=4;
    data=(char*)malloc(bpp*im.width*im.height);
    if(ImageByteOrder(d)==MSBFirst){
      for(int i=0;i<im.len;i++){
	*(data+i*4)=0;
	*(data+i*4+1)=im.data[i].r;
	*(data+i*4+2)=im.data[i].g;
	*(data+i*4+3)=im.data[i].b;
      }
    }else{
      for(int i=0;i<im.len;i++){
	*(data+i*4)=im.data[i].b;
	*(data+i*4+1)=im.data[i].g;
	*(data+i*4+2)=im.data[i].r;
	*(data+i*4+3)=0;
      }
    }
    break;
  default:
    assert(false);
  }
  //  cerr << depth << endl;
  assert(data!=NULL);
  img=XCreateImage(d,DefaultVisual(d,0),depth,ZPixmap,0,data,im.width,im.height,bpp*8,bpp*im.width);
  XPutImage(d,w,DefaultGC(d,0),img,0,0,0,0,im.width,im.height);
  XFlush(d);
  XDestroyImage(img);
  return 0;
}

static int root_height,root_width;
static int
ShowFile(Display *d,Window w,const string& f){
  data file;
  image im,im2;
  int r;
  cerr << f << endl;
  XStoreName(d,w,f.c_str());
  XMoveWindow(d,w,0,0);
  data_load(file,f.c_str());
  if(itu_yuv2image(file,image_height,image_width,im)<0){
    image_free(im);
    data_free(file);
    return -1;
  }
  data_free(file);
  if(root_height<im.height||root_width<im.width){
    if(((double)root_height/(double)root_width)<((double)im.height/(double)im.width))
      resize(im,im2,static_cast<int>(im.width*((double)root_height/(double)im.height)),root_height);
    else
      resize(im,im2,root_width,static_cast<int>(im.height*((double)root_width/(double)im.width)));  
    r=ShowImage(d,w,im2);
  }else
    r=ShowImage(d,w,im);
  
  image_free(im);
  image_free(im2);
  return r;
}

int
main(int argc,char** argv){
  string line;
  vector<string> flist;
  unsigned int i=0;
//   ifstream in("image.txt");
  istream& in=cin;
  if(getline(in,line))
    flist.push_back(line);
  if(flist.size()==0)
    return -1;

  if(argc!=3){
    image_width=640;
    image_height=480;
  }else{
    image_width=atoi(argv[1]);
    image_height=atoi(argv[2]);
  }
    

  
  Display *d;
  Window w;
  XEvent e;
  
  d=XOpenDisplay(NULL);
  int x,y;
  unsigned int wid,h,bw,dep;
  Window rw;
  XGetGeometry(d,RootWindow(d,0),&rw,&x,&y,&wid,&h,&bw,&dep);
  root_height=h;
  root_width=wid;
  w=XCreateSimpleWindow(d,RootWindow(d,0),0,0,wid,h,0,BlackPixel(d,0),BlackPixel(d,0));
  XMoveWindow(d,w,0,0);
  XSelectInput(d,w,KeyPressMask);
  XMapWindow(d,w);
  
  ShowFile(d,w,flist[0]);
  
  while(1){
    XNextEvent(d,&e);
    switch(e.type){
    case Expose:
      break;
    case KeyPress:
      switch(XKeycodeToKeysym(d,e.xkey.keycode,0)){
      case XK_q:
	return 0;
	break;
      case XK_space:
      case XK_f:
      case XK_n:
	if(i+1<flist.size())
	  i++;
	else if(i+1==flist.size()&&in){
	  if(getline(in,line)){
	    flist.push_back(line);
	    i++;
	  }
	}
	if(flist.size()<=i)
	  i=flist.size()-1;
	ShowFile(d,w,flist[i]);
	break;
      case XK_d:
      case XK_p:
	if(i-1>=0)
	  i--;
	if(flist.size()<=i)
	  i=flist.size()-1;
	ShowFile(d,w,flist[i]);
	break;
      case XK_s:
	if(i+10<flist.size())
	  i+=10;
	else{
	  for(int j=0;j<10;j++){
	    if(i+1==flist.size()&&in)
	      if(getline(in,line))
		flist.push_back(line);
	    i++;
	  }
	}
	if(flist.size()<=i)
	  i=flist.size()-1;
	ShowFile(d,w,flist[i]);
	break;
      case XK_a:
	if(i-10>=0)
	  i-=10;
	if(flist.size()<=i)
	  i=flist.size()-1;
	ShowFile(d,w,flist[i]);
	break;
      case XK_x:
	if(i+100<flist.size())
	  i+=100;
	else{
	  for(int j=0;j<100;j++){
	    if(i+1==flist.size()&&in)
	      if(getline(in,line))
		flist.push_back(line);
	    i++;
	  }
	}
	if(flist.size()<=i)
	  i=flist.size()-1;
	ShowFile(d,w,flist[i]);
	break;
      case XK_z:
	if(i-100>=0)
	  i-=100;
	ShowFile(d,w,flist[i]);
	break;
      case XK_v:
	if(i+1000<flist.size())
	  i+=1000;
	else{
	  for(int j=0;j<1000;j++){
	    if(i+1<=flist.size()&&in)
	      if(getline(in,line))
		flist.push_back(line);
	    i++;
	  }
	}
	if(flist.size()<=i)
	  i=flist.size()-1;
	ShowFile(d,w,flist[i]);
	break;
      case XK_c:
	if(i-1000>=0)
	  i-=1000;
	ShowFile(d,w,flist[i]);
	break;
      case XK_r:
	ShowFile(d,RootWindow(d,0),flist[i]);
	break;
      }
    }
  }
  return 0;
}

