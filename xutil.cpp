#include "xutil.h"
#include <cassert>
Display *xdisplay;
XEvent xevent;
unsigned int xroot_height,xroot_width;

int
xinit(){
  int x,y;
  unsigned int bw,dep;
  Display * &d=xdisplay;
  d=XOpenDisplay(NULL);
  Window rw;
  XGetGeometry(d,RootWindow(d,0),&rw,&x,&y,&xroot_width,&xroot_height,&bw,&dep);
  return 0;
}

int
create_window(Window& w){
  Display * &d=xdisplay;
  w=XCreateSimpleWindow(d,RootWindow(d,0),
			0,0,
			100,100,
//  			xroot_width,xroot_height,
			0,BlackPixel(d,0),BlackPixel(d,0));
//   XMoveWindow(d,w,0,0);
  XSelectInput(d,w,KeyPressMask|ExposureMask);
  XMapWindow(d,w);
  return 0;
}


int
image2Ximage(const image& im,XImage*& img){
  Display * &d=xdisplay;  
  int depth=DefaultDepth(d,0);
  int bpp=0;
  unsigned char *data;

  
  assert(im.width*im.height>0);

//   if(!(img!=NULL&&im.height==img->height&&im.width==img->width)){
  if(img!=NULL)
      XDestroyImage(img);
    img=NULL;
    switch(depth){
    case 16:
      bpp=2;
      break;
    case 24:
      bpp=4;
      break;
    default:
      assert(false);
    }
    data=(unsigned char*)malloc(bpp*im.width*im.height);
    img=XCreateImage(d,DefaultVisual(d,0),
		     depth,ZPixmap,0,(char*)data,
		     im.width,im.height,
		     bpp*8,bpp*im.width);
//   }

  
  switch(depth){
  case 16:
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
  
  return 0;  
}

int
Ximage_show(XImage* img,Window& w){
  Display * &d=xdisplay;
  if(img!=NULL){
    XPutImage(d,w,DefaultGC(d,0),img,0,0,0,0,img->width,img->height);
    XFlush(d);
  }
  return 0;
}
