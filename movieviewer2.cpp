#include "image.h"
#include "movie.h"
#include "yuv_util.h"

#include "xutil.h"

#include <cstdio>
#include <map>
#include <string>
#include <fstream>
using namespace std;



#define MINMAX(value,min,max) ((value)>(max)?(max):((value)<(min)?(min):(value)))

struct Mode{
  static int diff;//0:pixel,1:macro,-1,2:none
  static int scale;
  static int (*(diff_table[]))(const image&,const image&,image& diff_im,unsigned int);
  static int diff_hist;//0:pixel,1:macro,-1,2:none
  static int (*(diff_hist_table[]))(const image&,const image&,
				    std::vector<int>&,
				    std::vector<int>&,
				    std::vector<int>&
				    );

  static int rgb;//0:none,0:r,1:g,2:b
  static int (*( rgb_table[]))(image&);

  
  static int gradation;//0:gray,1:color
  static int (*(gradation_table[]))(const std::vector<RGB>&,image&);
  
  static int reverse;//0:none,1:reverse
  static int (*(reverse_table[]))(image&);

  static int mask;//0:none,1:mask
  static int (*(mask_table[]))(const image&,image&);

  static int threshold;//0:none,1:mask
  static int threshold_min,threshold_max;
  static int (*(threshold_table[]))(image&,int,int);

  static int algo;//0:none,1:mask
  static int (*(algo_table[]))(image&);

  static int edge;//0:none,1:mask
  static int (*(edge_table[]))(image&);

  static int hist;//0:none,1:mask
  static int (*(hist_table[]))(image&);
  
  static int convert(image& im1,image& im2
		     ,image& im){

    if(algo!=-1)
      if(algo_table[algo]!=NULL)
	algo_table[algo](im1);
      else
	algo=-1;

    if(edge!=-1)
      if(edge_table[edge]!=NULL){
	edge_table[edge](im1);
	edge_table[edge](im2);
      }else
	edge=-1;

    image_copy(im1,im);
    
    
    if(diff!=-1)
      if(diff_table[diff]!=NULL)
	diff_table[diff](im1,im2,im,scale);
      else
	diff=-1;

//     if(diff_hist!=-1)
//       if(diff_hist_table[diff_hist]!=NULL)
// 	diff_hist_table[diff_hist](im1,im2,im1);
//       else
// 	diff_hist=-1;

    if(rgb!=-1)
      if(rgb_table[rgb]!=NULL)
	rgb_table[rgb](im);
      else
	rgb=-1;


    if(reverse!=-1)
      if(reverse_table[reverse]!=NULL)
	reverse_table[reverse](im);
      else
	reverse=-1;


    if(threshold!=-1)
      if(threshold_table[threshold]!=NULL)
	threshold_table[threshold](im,threshold_min,threshold_max);
      else
	threshold=-1;

    if(mask!=-1)
      if(mask_table[mask]!=NULL)
	mask_table[mask](im1,im);
      else
	mask=-1;

    if(hist!=-1)
      if(hist_table[hist]!=NULL)
	hist_table[hist](im);
      else
	hist=-1;
    return 0;

//     if(gradation!=-1)
//       if(gradation_table[gradation]!=NULL)
// 	gradation_table[gradation](im);
//       else
// 	gradation=-1;
  }
};
int Mode::diff=-1;
int (*(Mode::diff_table[]))(const image&,const image&,image& diff_im,unsigned int)
  ={yuv_diff_pix,yuv_diff_macro,NULL};
int Mode::scale=1;

int Mode::diff_hist=-1;
int (*(Mode::diff_hist_table[]))(const image&,
				 const image&,
				 std::vector<int>&,
				 std::vector<int>&,
				 std::vector<int>&
				 )
  ={yuv_sort_diff_pix,yuv_sort_diff_macro,NULL};

int Mode::rgb=-1;
int (*(Mode::rgb_table[]))(image&)
  ={rgb_r_gray,rgb_g_gray,rgb_b_gray,rgb_max_gray,NULL};

int Mode::gradation=-1;
int (*(Mode::gradation_table[]))(const std::vector<RGB>&,image&)
  ={rgb_gradation,NULL};

int Mode::reverse=-1;
int (*(Mode::reverse_table[]))(image&)
  ={rgb_reverse,NULL};

int Mode::mask=-1;
int (*(Mode::mask_table[]))(const image&,image&)
  ={rgb_and,NULL};


int Mode::threshold=-1;
int (*(Mode::threshold_table[]))(image&,int,int)
  ={rgb_threshold,NULL};
int Mode::threshold_min=0;
int Mode::threshold_max=255;


int Mode::algo=-1;
int (*(Mode::algo_table[]))(image&)
  ={rgb_median,rgb_average,rgb_sharp,NULL};

int Mode::edge=-1;
int (*(Mode::edge_table[]))(image&)
  ={rgb_rap,NULL};

int Mode::hist=-1;
int (*(Mode::hist_table[]))(image&)
  ={rgb_hist,NULL};



int
main(int argc,char** argv){
  Display * &d=xdisplay;
  XEvent &e=xevent;
  movie mo,mo2;
  image im1,im2,im;
  int i=0;
  char title[2048];
  Window w;
  XImage* img=NULL;
  
  yuv2movie(argv[1],atoi(argv[2]),atoi(argv[3]),1,mo);
  if(argc==5)
    yuv2movie(argv[4],atoi(argv[2]),atoi(argv[3]),1,mo2);

  xinit();
  create_window(w);
  XMoveResizeWindow(d,w,0,0,640,480);  
  goto BEGIN;
  while(1){
    XNextEvent(d,&e);
    switch(e.type){
    case Expose:
      Ximage_show(img,w);
      break;
    case KeyPress:
      switch(XKeycodeToKeysym(d,e.xkey.keycode,0)){
      case XK_q:
	return 0;
	break;
      case XK_space:
      case XK_n:
	i++;
	break;
      case XK_p:
	i--;
	break;
      case XK_z:
	i-=10;
	break;
      case XK_x:
	i+=10;
	break;
      case XK_c:
	i-=100;
	break;
      case XK_v:
	i+=100;
	break;
      case XK_h:
	Mode::hist++;
	break;
      case XK_a:
	Mode::algo++;
	break;
      case XK_e:
	Mode::edge++;
	break;
      case XK_t:
	Mode::threshold++;
	break;
      case XK_y:
	Mode::threshold_min-=10;
	break;
      case XK_u:
	Mode::threshold_min+=10;
	break;
      case XK_i:
	Mode::threshold_max-=10;
	break;
      case XK_o:
	Mode::threshold_max+=10;
	break;
      case XK_j:
	Mode::scale++;
	break;
      case XK_k:
	Mode::scale--;
	break;
      case XK_d:
	Mode::diff++;
	break;
      case XK_r:
	Mode::rgb++;
	break;
      case XK_b:
	Mode::reverse++;
	break;
      case XK_m:
	Mode::mask++;
	break;
      default:
	continue;
      }
    BEGIN:
      i=MINMAX(i,0,mo.end_pos-1);
      MOVIE_READ(mo,i,im1);
      MOVIE_READ(mo2,i,im2);
      
      Mode::convert(im1,im2,im);

      image2Ximage(im,img);
      image_free(im);
      image_free(im1);
      image_free(im2);
      Ximage_show(img,w);
      sprintf(title,
	      "frame:%6d "
	      "algo:%d "
	      "edge:%d "
	      "diff:%d,scale:%d "
	      "rgb:%d "
	      "thresh:%d,min:%d,max:%d "
	      "mask:%d "
	      "rev:%d "
	      ,i
	      ,Mode::algo
	      ,Mode::edge
	      ,Mode::diff,Mode::scale
	      ,Mode::rgb
	      ,Mode::threshold,Mode::threshold_min,Mode::threshold_max,
	      Mode::mask,
	      Mode::reverse);
      XStoreName(d,w,title);
    }
  }

  
  return 0;
}
