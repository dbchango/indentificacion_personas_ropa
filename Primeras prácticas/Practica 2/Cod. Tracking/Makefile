LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui
INCLUDES = /usr/local/include

all: vats

vats: vats_tracking.o main.o
	gcc -o vats main.o vats_tracking.o $(LIBS) -lm

main.o: main.c
	gcc -c main.c -I$(INCLUDES) -O2

vats_tracking.o: vats_tracking.c
	gcc -c vats_tracking.c -I$(INLCUDES) -O2
