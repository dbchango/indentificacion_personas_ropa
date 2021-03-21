LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video -lopencv_ml
INCLUDES = /usr/include/opencv-2.3.1/

all: vats

vats: main.o BlobList.o BasicBlob.o blobstationary.o blobextract.o blobclassify.o redn.o
	gcc -o vats main.o BlobList.o BasicBlob.o blobstationary.o blobextract.o blobclassify.o redn.o $(LIBS) -lm

main.o: main.cpp
	gcc -c main.cpp -I$(INCLUDES) -O
	
BlobList.o: BlobList.cpp
	gcc -c BlobList.cpp -I$(INCLUDES) -O
	
BasicBlob.o: BasicBlob.cpp
	gcc -c BasicBlob.cpp -I$(INCLUDES) -O
	
blobstationary.o: blobstationary.cpp
	gcc -c blobstationary.cpp -I$(INCLUDES) -O
	
blobextract.o: blobextract.cpp
	gcc -c blobextract.cpp -I$(INCLUDES) -O
	
blobclassify.o: blobclassify.cpp
	gcc -c blobclassify.cpp -I$(INCLUDES) -O

redn.o: redn.cpp
	gcc -c redn.cpp -I$(INCLUDES) -O

