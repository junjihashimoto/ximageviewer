#ifndef XUTILCPP_H
#define XUTILCPP_H
extern "C"{
#ifdef W11
#include <W11/Xlib.h>
#undef DefaultDepth
#define DefaultDepth(a,b) 24
#define XDestroyImage(ximage) ((*((ximage)->f.destroy_image))((ximage)))
#undef RGB

#else //W11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#endif //W11 
}
#include "image.h"

extern Display *xdisplay;
extern XEvent xevent;
extern unsigned int xroot_height,xroot_width;
extern int xinit();
extern int create_window(Window& w);
extern int image2Ximage(const image& im,XImage*& img);
extern int Ximage_show(XImage* img,Window& w);

#endif // XUTILCPP_H
