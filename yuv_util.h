#ifndef YUVUTILCPP_H
#define YUVUTILCPP_H
#include <vector>
#include "image.h"

//diff.cpp
extern int yuv_diff_pix(const image& ori1,const image& ori2,image& diff_im,
			unsigned int scale=1);
extern int yuv_diff_macro(const image& ori1,const image& ori2,image& diff_im,
			  unsigned int scale=1);
extern int yuv_diff_block(const image& ori1,const image& ori2,image& diff_im,
			  unsigned int scale=1);

extern int rgb_gradation(const std::vector<RGB>& color_table,image& diff_im);

extern int yuv_y_gray(image& diff_im);
extern int yuv_u_gray(image& diff_im);
extern int yuv_v_gray(image& diff_im);

extern int rgb_r_gray(image& diff_im);
extern int rgb_g_gray(image& diff_im);
extern int rgb_b_gray(image& diff_im);
extern int rgb_max_gray(image& diff_im);

extern int rgb_threshold(image& diff_im,int min,int max);
extern int rgb_reverse(image& diff_im);
extern int rgb_and(const image& mask,image& diff_im);

extern int rgb_change(image& ori1,image& ori2);

extern int yuv_sort_diff_pix(const image& ori1,const image& ori2,
			  std::vector<int>& diff_y,
			  std::vector<int>& diff_u,
			  std::vector<int>& diff_v
			  );
extern int yuv_sort_diff_macro(const image& ori1,const image& ori2,
			  std::vector<int>& diff_y,
			  std::vector<int>& diff_u,
			  std::vector<int>& diff_v
			  );

//filter.cpp
extern int rgb_median(image& im);
extern int rgb_average(image& im);
extern int rgb_sharp(image& im);
extern int rgb_rap(image& im);
extern int rgb_hist(image& im);
extern int rgb_resize(image& im,int width,int height);
extern int rgb_trim(image& im,int max_width,int max_height);

//

#endif //YUVUTILCPP_H
