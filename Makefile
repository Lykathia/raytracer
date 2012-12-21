# Simple Makefile for raycasting assignment
# for g++ on linux platforms

# ===============================================================

SRCS 	= main.cpp \
		  matrix.C \
		  scene_parser.cpp \
		  sphere.cpp \
          triangle.cpp \
          plane.cpp \
		  camera.cpp \
          raytracer.cpp \
          transform.cpp \
		  group.cpp

# ===============================================================

LIBS    = -lm -L/usr/X11R6/lib -lpng
CC		= g++
EXE	  	= raytracer
OBJS =  $(SRCS:.C=.o)

# ===============================================================
# targets

.PHONY: all depend clean

all: depend $(EXE)

depend:
	$(CC) -E -M $(SRCS) > Makefile.depend

clean: 
	-rm -f *~ *bak *.o  core.* Makefile.depend $(EXE) 

# ===============================================================
# compilation rules

$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

.C.o:
	$(CC) $< -c -o $@

.cpp.o:
	$(CC) $< -c -o $@

# ===============================================================

-include Makefile.depend

