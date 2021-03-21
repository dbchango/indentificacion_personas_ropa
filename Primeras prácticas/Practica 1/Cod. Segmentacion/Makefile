LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui
INCLUDES = /usr/local/include

all: main

main: impfunseg.o main.o
	gcc -o main main.o impfunseg.o $(LIBS) -lm

main.o: decvarseg.h main.c
	gcc -c main.c -I$(INCLUDES) -O2

impfunseg.o: decvarseg.h impfunseg.c
	gcc -c impfunseg.c -I$(INLCUDES) -O2
