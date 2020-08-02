CFLAGS=-O4
LIBS=-L/usr/X11R6/lib \
   -lgif -lpng -ljpeg  \
   -lX11 -ldl -lxcb -lpthread -lXau -lXdmcp -lz -static 
DEPENDEDOBJS=
CXX=g++
OBJS=xutil.o filter.o data.o image.o check.o gif.o jpeg.o bmp.o png.o ppm.o yuv.o diff.o yuv_movie.o
PROGRAMS=xviewer findviewer yuvviewer movieviewer yuvmovie2bmp
PROJECT=xviewer

all:$(PROGRAMS)

xviewer:viewer.o $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $< $(OBJS) $(LIBS)
findviewer:findviewer.o $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $< $(OBJS) $(LIBS)

yuvviewer:yuvviewer.o $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $< $(OBJS) $(LIBS)
movieviewer:movieviewer.o $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $< $(OBJS) $(LIBS)
yuvmovie2bmp:yuvmovie2bmp.o $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $< $(OBJS) $(LIBS)

%.o:%.cpp
	$(CXX) $(CFLAGS) -c $<
clean:
	-rm *.o $(PROGRAMS)
dist:
	tar cvfz $(PROJECT).tgz $(SRC)
