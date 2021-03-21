//file description
/**
 * \file main.cpp 
 * \author Juan Carlos San Miguel
 * \date 09/03/2013
 * \brief Scheme for lab sessions of VATS (I2TIC Master - EPS (UAM)
 * \version 1.0\n 
 *
 */

//standard and OpenCV functions
#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "opencv/cv.h"
#include "opencv/cvaux.h"
#include "opencv/highgui.h"
#include "opencv2/video/background_segm.hpp"

//include for blob-related functions
#include "blobfuns.h"

#define INPUT_VIDEO	"pe.mpg"

int main()
{	
	// Required variables for the program
	CvCapture* capture=NULL;
	IplImage *frame=NULL, *fg=NULL, *bg=NULL; //images for background subtraction	
	IplImage *fgcounter=NULL, *sfg=NULL; //images for stationary foreground analysis (counter and static fg mask)
	IplImage *outblobs=NULL, *outlabels=NULL; //output images for blob extraction and blob labels
	
	CvVideoWriter *videowriter;
	
	double start=0,end=0,total=0;	
	int i = 0;
	CvScalar white = {255};

	CvFont font;
	char buf[100];
	
	//read video file & first frame
	if(!(capture = cvCaptureFromFile(INPUT_VIDEO)))
	{
		printf("Bad Video File\n");
		return -1;
	}
	
	frame = cvQueryFrame( capture );
	
	//image initialization (counters, auxiliar images,...)
	//...

	int umbral = 25;
	int umbfan = 20;
	int width = 720;
	int height = 400;
	float alfa = 0.05;


	//module initialization (background subtraction, bloblist,...)
	//...

    	CvGaussBGStatModelParams *params = new CvGaussBGStatModelParams;
	params->win_size = 50;
	params->n_gauss = 5;
	params->bg_threshold = 0.7;
	params->std_threshold = 3.5;
	params->minArea = 15;
	params->weight_init = 0.05;
	params->variance_init = 30;

	CvBGStatModel *bgModel = cvCreateGaussianBGModel (frame, params);
	

	IplImage *ImgFrist = cvCreateImage(cvGetSize(frame), frame->depth, 1);
   	cvCvtColor(frame, ImgFrist, CV_RGB2GRAY);
   
        sfg = cvCreateImage(cvGetSize(ImgFrist),ImgFrist->depth,1);
        //sfg = cvCloneImage(ImgFrist);     //Imagen de Confianza
        cvSetZero(sfg);

	//fgcounter = cvCreateImage(cvGetSize(ImgFrist),ImgFrist->depth,1);
	fgcounter  = cvCloneImage(ImgFrist);
        cvSetZero(fgcounter);
   
   	cvNamedWindow("Frente",0);

	BlobList *blobList = new BlobList();

	//create output windows	
	//...

        cvNamedWindow("Imagen Original",0);
//	cvNamedWindow("FloodFill",0); 

	//create output writer
	videowriter = cvCreateVideoWriter("result.mpg", CV_FOURCC('P','I','M','1'), 25, cvGetSize(frame), 1 );	
	cvInitFont( &font, CV_FONT_HERSHEY_DUPLEX, 0.8, 0.8, 0, 2, 8 );
	
	do
	{
		i++;
		start =((double)cvGetTickCount()/(cvGetTickFrequency()*1000.) );

		//background subtraction (final foreground mask must be placed in 'fg' variable)
		//...
		
		if (!frame)
			break;

		cvUpdateBGStatModel(frame, bgModel);
   		cvShowImage("Imagen Original", frame);
		fg = bgModel->foreground;

      		cvShowImage("Frente", fg);

      		//blob extraction
		extractBlobs(frame, fg, blobList); //blob extraction
		//outblobs = paintBlobImage(frame, blobList);//paint blobs

		//blob classification
		classifyBlobs(frame, fg, blobList); //classify blobs
		//outlabels = paintBlobClasses(frame, blobList);//paint classification blobs
		
		//stationary blob detection

		detectStationaryForeground(frame, fg, fgcounter, sfg);

		// Drawing and text functions for frame
		/*cvRectangle( frame, cvPoint(10,10), cvPoint(frame->width-10, frame->height-10), white, 2, 8, 0 );
		sprintf(buf,"VATS");
		cvPutText(frame, buf, cvPoint(20,frame->height-30), &font, white );*/

		//show results visually
		//...
	
		end = ((double)cvGetTickCount()/(cvGetTickFrequency()*1000.) );
		total=total + end-start;

		//printf("Processing frame %d --> %.3g ms\n", i,end-start);		
		if (cvWaitKey(33) == 27)
                   break;

		//write frame result to video
		
		//cvWriteFrame( videowriter, outblobs );
		
		//release memory of temporal images
		//cvReleaseImage( &outblobs );
		//cvReleaseImage( &outlabels );

	} while (frame=cvQueryFrame( capture ));
		
	//destroy all resources
	//...
	cvReleaseImage(&ImgFrist);
	cvReleaseImage(&bg);
   	cvReleaseImage(&fg);
        cvReleaseBGStatModel(&bgModel);
	cvReleaseCapture(&capture);

	return 1;
}
