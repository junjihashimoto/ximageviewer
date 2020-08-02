
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

#include "image.h"
#include "yuv_util.h"
#include "xutil.h"


int
read_image(const string& fname,image& im){
  string line;
  data file;
  if(data_load(file,fname.c_str())<0)
    return -1;
  if(data2image(file,im)<0){
    data_free(file);
    return -1;
  }
  data_free(file);
  return 0;
}

int
read_list(istream& in,vector<string>& flist,image& im){
  string line;
  data file;
  while(getline(in,line)){
    if(data_load(file,line.c_str())<0)
      continue;
    if(data2image(file,im)>=0){
      data_free(file);
      flist.push_back(line);
      return 0;
    }
    data_free(file);
  }
  return -1;
}
bool
checkdirp(const string& str){
  unsigned int pos=str.rfind('/');
  if(pos==string::npos){
    return false;
  }
  return str.find('.',pos)==string::npos;
}

int
read_list(istream& in,vector<string>& flist,int i,bool mode){
  string line;
  if(i<0)
    return -1;
  while(i!=0&&getline(in,line)){
    if(checkdirp(line)==true)
      continue;
    if(mode&&check_data(line.c_str())<0)
      continue;
    flist.push_back(line);
    i--;
  }
  return 0;
}
int
prev_dir(vector<string>& flist,unsigned int i){
  if(i>0)
    i--;
  unsigned int pos=flist[i].rfind('/');
  string dirstr="";
  if(pos==string::npos){
    return 0;
  }
  if(checkdirp(flist[i]))
    dirstr=flist[i];
  else
    dirstr=flist[i].substr(0,pos);
  for(;i>0;i--){
    string str=flist[i].substr(0,dirstr.length());
    if(str!=dirstr)
      break;
  }
  if(i!=0&&i!=flist.size()-1)
    i++;
  
  return i;
}
int
next_dir(const vector<string>& flist,unsigned int i){
  unsigned int pos=flist[i].rfind('/');
  string dirstr="";
  if(pos==string::npos){
    return 0;
  }
  if(checkdirp(flist[i]))
    dirstr=flist[i];
  else
    dirstr=flist[i].substr(0,pos);
  for(;i<flist.size();i++){
     string str=flist[i].substr(0,dirstr.length());
     if(str!=dirstr&&checkdirp(flist[i])==false){
       break;
     }
  }
  if(i==flist.size())
    i--;
  return i;
}

int
main(){
  string line;
  vector<string> flist;
  int i=0;
  //  ifstream in("/home/junji/list");
  image im;
  //   ifstream in("image.txt");
  istream& in=cin;
  bool mode=false;
  
  Display * &d=xdisplay;
  XEvent &e=xevent;
  Window w;
  XImage *img=NULL;

  xinit();
  xroot_height-=32;
  create_window(w);

  if(read_list(in,flist,im)<0)
    return -1;
//   if(read_list(in,flist,1)<0)
//     return 1;
//   if(read_image(flist[0],im)<0)
//     return 1;
  cout << flist[0] << endl;

  
  rgb_trim(im,xroot_width,xroot_height);
  image2Ximage(im,img);
  XMoveResizeWindow(d,w,0,0,im.width,im.height);
  image_free(im);

  //  XMoveWindow(d,w,0,0);
  XSelectInput(d,w,KeyPressMask|ExposureMask );
  Ximage_show(img, w);
  XMapWindow( d, w );

//   return 0;


  goto BEGIN;
  while(1){
    XNextEvent(d,&e);
    switch(e.type){
    case Expose:
      Ximage_show(img, w);
      XFlush( d );
      break;
    case KeyPress:
      switch(XKeycodeToKeysym(d,e.xkey.keycode,0)){
      case XK_q:
	return 0;
      case XK_n:
	read_list(in,flist,i+1000+1-flist.size(),mode);
	i=next_dir(flist,i);
	break;
      case XK_p:
	read_list(in,flist,i+1000+1-flist.size(),mode);
	i=prev_dir(flist,i);
	break;
      case XK_f:
	i++;
	break;
      case XK_d:
	i--;
	break;
      case XK_s:
	i+=10;
	break;
      case XK_m:
	mode=(mode==true?false:true);
	break;
      case XK_a:
	i-=10;
	break;
      case XK_v:
	i+=100;
	break;
      case XK_c:
	i-=100;
	break;
      case XK_x:
	i+=1000;
	break;
      case XK_z:
	i-=1000;
	break;
      case XK_r:
	i+=10000;
	break;
      case XK_e:
	i-=10000;
	break;
      case XK_space:
	if(i>=(int)flist.size())
	  printf("Out of index:%d\n",i);
	else
	  printf("%s\n",flist[i].c_str());
	break;
      default:
	continue;
      }
    BEGIN:
      if(i<0)
	i=0;
      else{
	read_list(in,flist,i+1-flist.size(),mode);
	if(i>=(int)flist.size())
	  i=flist.size()-1;
      }
      XStoreName(d,w,flist[i].c_str());
      //      cout << i << " "<<flist[i] << endl;
      if(read_image(flist[i],im)<0)
	break;
      rgb_trim(im,xroot_width,xroot_height);
      image2Ximage(im,img);
      XMoveResizeWindow(d,w,0,0,im.width,im.height);
      image_free(im);
      
      Ximage_show(img,w);
    }
  }
  return 0;
}

