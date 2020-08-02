#include "image.h"
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
extern "C"{
#include <gif_lib.h>
}
using namespace std;

struct data_buf{
  data* file;
  int pos;
};

static void PrintGifError(int err)
{
  const char *Err = GifErrorString(err);

  if (Err != NULL)
    fprintf(stderr, "\nGIF-LIB error: %s.\n", Err);
  else
    fprintf(stderr, "\nGIF-LIB undefined error .\n");
}

static int
ungif_input_func(GifFileType *GifFile, GifByteType *p, int s)
{
  data_buf *st = (data_buf *)GifFile->UserData;
  int ret;
  if((st->file->len-st->pos)>s){
    ret=s;
    for(int i=0;i<ret;i++)
      p[i]=*(st->file->head+st->pos+i);
    st->pos+=s;
  }else{
    ret=(st->file->len-st->pos);
    for(int i=0;i<ret;i++)
      p[i]=*(st->file->head+st->pos+i);
    st->pos=st->file->len;
  }
  return ret;
}
int
gif2image(const data& file,image& im){
  int size, col, extcode;
  unsigned int row, sheight;
  int ioffset[] = { 0, 4, 2, 1 };
  int ijumps[] = { 8, 8, 4, 2 };
  GifRecordType RecordType;
  GifByteType *Extension;
  GifRowType *ScreenBuffer;
  GifFileType *GifFile;
  ColorMapObject *ColorMap;
  int image_loaded = 0;
  data_buf buf = {const_cast<data*>(&file),0};
  int err;
  if ((GifFile = DGifOpen((void*)&buf, ungif_input_func, &err)) == NULL) {
#ifdef DEBUG
    PrintGifError(err);
#endif
    return -1;
  }

  sheight = GifFile->SHeight;

  if ((ScreenBuffer = (GifRowType *)calloc(sheight, sizeof(GifRowType *))) == NULL) {
    if (DGifCloseFile(GifFile, &err) == GIF_ERROR)
      PrintGifError(err);
    return -1;
  }

  size = GifFile->SWidth * sizeof(GifPixelType);
  if ((ScreenBuffer[0] = (GifRowType)calloc(sheight, size)) == NULL) {
    free(ScreenBuffer);
    if (DGifCloseFile(GifFile, &err) == GIF_ERROR)
      PrintGifError(err);
    return -1;
  }
  for(unsigned int i=1;i<sheight;i++)
    ScreenBuffer[i]=ScreenBuffer[0]+i*size;
  memset(ScreenBuffer[0], GifFile->SBackGroundColor, sheight * size);
  //  p->background_color.index = GifFile->SBackGroundColor;

  do {
    if (DGifGetRecordType(GifFile, &RecordType) == GIF_ERROR) {
      if (!image_loaded){
	PrintGifError(err);
	DGifCloseFile(GifFile, &err);
	free(ScreenBuffer[0]);
	free(ScreenBuffer);
	return -1;
      }
      /* Show error message, but try to display. */
      PrintGifError(err);
      break;
    }

    switch (RecordType) {
    case IMAGE_DESC_RECORD_TYPE:
      if (image_loaded) {
	DGifCloseFile(GifFile, &err);
	free(ScreenBuffer[0]);
	free(ScreenBuffer);
	return -1;
      }
      image_loaded++;

      if (DGifGetImageDesc(GifFile) == GIF_ERROR){
	PrintGifError(err);
	DGifCloseFile(GifFile, &err);
	free(ScreenBuffer[0]);
	free(ScreenBuffer);
	return -1;
      }

	
      row = GifFile->Image.Top;
      col = GifFile->Image.Left;
      im.width = GifFile->Image.Width;
      im.height = GifFile->Image.Height;
      im.len=im.width*im.height;

      if (GifFile->Image.Interlace) {
	for (int i=0;i<4;i++)
	  for (unsigned int j=row+ioffset[i];j<row+im.height;j+=ijumps[i])
	    if (DGifGetLine(GifFile, &ScreenBuffer[j][col], im.width) == GIF_ERROR){
	      PrintGifError(err);
	      DGifCloseFile(GifFile, &err);
	      free(ScreenBuffer[0]);
	      free(ScreenBuffer);
	      return -1;
	    }
      } else {
	for (int i=0;i<im.height;i++)
	  if (DGifGetLine(GifFile, &ScreenBuffer[row+i][col], im.width) == GIF_ERROR){
	    PrintGifError(err);
	    DGifCloseFile(GifFile, &err);
	    free(ScreenBuffer[0]);
	    free(ScreenBuffer);
	    return -1;
	  }
      }
      break;
    case EXTENSION_RECORD_TYPE:
      if (DGifGetExtension(GifFile, &extcode, &Extension) == GIF_ERROR){
	PrintGifError(err);
	DGifCloseFile(GifFile, &err);
	free(ScreenBuffer[0]);
	free(ScreenBuffer);
	return -1;
      }
      for (;;) {
	if (DGifGetExtensionNext(GifFile, &Extension) == GIF_ERROR){
	  PrintGifError(err);
	  DGifCloseFile(GifFile, &err);
	  free(ScreenBuffer[0]);
	  free(ScreenBuffer);
	  return -1;
	}
	if (Extension == NULL)
	  break;
      }
      break;
    default:
      break;
    }
  } while (RecordType != TERMINATE_RECORD_TYPE);

  ColorMap = GifFile->Image.ColorMap ? GifFile->Image.ColorMap : GifFile->SColorMap;
  GifByteType colormap[ColorMap->ColorCount][3];

  for (int i=0;i< ColorMap->ColorCount;i++) {
    colormap[i][0]=ColorMap->Colors[i].Red;
    colormap[i][1]=ColorMap->Colors[i].Green;
    colormap[i][2]=ColorMap->Colors[i].Blue;
  }

  if (DGifCloseFile(GifFile, &err) == GIF_ERROR){
    PrintGifError(err);
    DGifCloseFile(GifFile, &err);
    free(ScreenBuffer[0]);
    free(ScreenBuffer);
    return -1;
  }

  im.data=(RGB*)malloc(im.width*im.height*sizeof(RGB));
  assert(im.data!=NULL);
  for (int i=0;i<im.height;i++){
    for(int j=0;j<im.width;j++){
      im.data[i*im.width+j].r=colormap[ScreenBuffer[row+i][col+j]][0];
      im.data[i*im.width+j].g=colormap[ScreenBuffer[row+i][col+j]][1];
      im.data[i*im.width+j].b=colormap[ScreenBuffer[row+i][col+j]][2];
    }
  }

  free(ScreenBuffer[0]);
  free(ScreenBuffer);

  return 0;
}

#ifdef GIFCPP_MAIN
int
main(){
  data file;
  image im;
  data_load(file,"/dev/stdin");
  gif2image(file,im);
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
  //  std::reverse(im.data.begin(),im.data.end());
  image2ppm(im,file);
  data_save(file,"/dev/stdout");
  return 0;
}
#endif
