
#include <cstdio>
#include <map>
#include <iostream>
using namespace std;

#include "image.h"
#include "yuv_util.h"
#include "xutil.h"


int
main(){
  data file;
  image im,im2;
//   data_load(file,"sample.ppm");
  data_load(file,"/dev/stdin");
  // data_load(file,"imege2/t_ania01.gif");
  if(data2image(file,im)<0){
    cerr << "unsupported file" << endl;
    data_free(file);
    return -1;
  }
    
  Display * &d=xdisplay;
  XEvent &e=xevent;
  Window w;

  xinit();
  
  create_window(w);
  
  XImage *img=NULL;
//   rgb_resize(im,100,100);
  rgb_trim(im,xroot_width,xroot_height);
  
  image2Ximage(im,img);

  XSelectInput(d,w,KeyPressMask|ExposureMask );

  XMapWindow( d, w );
  XMoveResizeWindow(d,w,0,0,im.width,im.height);

  Ximage_show(img, w);
  XFlush( d );
  
  while(1){
    XNextEvent(d,&e);
    switch(e.type){
    case Expose:
//       cerr << "Expose" <<endl;
      Ximage_show(img, w);
      XFlush( d );
      break;
    case KeyPress:
//     case KeyRelease:
      switch(XKeycodeToKeysym(d,e.xkey.keycode,0)){
      case XK_q:
	return 0;
//       case XK_t:
// 	cerr << "press key" <<endl;
// 	break;
      }
      break;
    }
  }
  return 0;
}

