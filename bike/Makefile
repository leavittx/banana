# $Id: bike, 2009/05/05 17:32:32 $
CFLAGS=-march=native -O2 -pipe -lm -l GLU  -l glut

all: bike
bike: T45BIKE.o tex.o
T45BIKE.o: T45BIKE.c tex.c tex.h
tex.o: tex.c tex.h

#bike: T45BIKE.o tex.o
#T45BIKE.o tex.o: tex.h

clean:
	rm -f bike *.o
#	rm bike *.o
