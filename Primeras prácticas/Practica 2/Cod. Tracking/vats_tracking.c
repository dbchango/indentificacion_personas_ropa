#include "vats_tracking.h"

void inicializar_imagenes_scaladas(tTrackingVATS *descriptor)
{
   int n, m, i;
   char framename[255];
   char name[255];
   char buffer[10];

   i = 1;

   while (i < descriptor->num_comp){
      strcpy(name, "simple");
      snprintf(buffer, 10, "%d", i);
      strcat(name, buffer);
      strcpy(framename, "");
      n=sprintf(framename, "%s.jpg", name);
    
      descriptor->tmp[i] = cvLoadImage(framename, CV_LOAD_IMAGE_COLOR);
      
      Rtmp[i] = cvCreateImage(cvGetSize(descriptor->tmp[i]), IPL_DEPTH_8U, 1);
      cvSplit(descriptor->tmp[i], NULL, Rtmp[i], NULL, NULL);

      m++;
      i++;
   }
}


void my_mouse_callback( int event, int x, int y, int flags, void* param ){

   IplImage* image = (IplImage*) param;
   
   switch( event ){

      case CV_EVENT_LBUTTONDOWN:
	 //if (drawing_box == 0){
	    Pt1.x = x;
	    Pt1.y = y;
         //   drawing_box = 1;
         //}
	 break;

      case CV_EVENT_RBUTTONDOWN:
	 //if (drawing_box == 1){
	    Pt2.x = x;
            Pt2.y = y;
         //   drawing_box = 0;
         //}
	 break;

   }
}

void scalar_imagenes(int porcentaje, tTrackingVATS *descriptor){

   int n;
   char framename[255];
   char name[255];
   char buffer[10];
   
   IplImage *aux, *ImgScala;
   aux = cvLoadImage("simple1.jpg", CV_LOAD_IMAGE_COLOR);
   ImgScala = cvCreateImage (cvSize((int)((aux->width*porcentaje)/100) , (int)((aux->height*porcentaje)/100)), aux->depth, aux->nChannels );

   cvResize(aux, ImgScala, CV_INTER_LINEAR);   

   strcpy(name, "simple");
   snprintf(buffer, 10, "%d", descriptor->num);
   strcat(name, buffer);
   strcpy(framename, "");
   n=sprintf(framename, "%s.jpg", name);

   cvSaveImage (framename, ImgScala, 0);
   descriptor->num++;

   cvReleaseImage(&ImgScala);
}


void rescalar_imagenes(tTrackingVATS *descriptor){
    scalar_imagenes(50, descriptor);
    scalar_imagenes(75, descriptor);
    scalar_imagenes(150, descriptor);
    scalar_imagenes(200, descriptor);
    scalar_imagenes(250, descriptor);
    scalar_imagenes(300, descriptor);
}


void detener_frame(char *movie, tTrackingVATS *descriptor){

   CvCapture *capture1;
   capture1 = cvCaptureFromFile(movie);
   IplImage *img;
   descriptor->num_img = 0;
   cvNamedWindow("video",0);

   while (1){
      img = cvQueryFrame(capture1);
      if (!img)
         break;
      descriptor->num_img++;

      cvShowImage("video",img);
      if (cvWaitKey(33)==27)
         break;
   }

   cvSaveImage ("simple.jpg",img, 0);
   cvReleaseCapture(&capture1);

}


void captura_patron_imagen(tTrackingVATS *descriptor){

   IplImage *img;
   img = cvLoadImage("simple.jpg", CV_LOAD_IMAGE_COLOR);
   
   //cvNamedWindow("video",0);
   //descriptor->drawing_box = 0;

   while (1){

      cvShowImage("video", img);
      cvSetMouseCallback( "video", my_mouse_callback, (void*) img);

      if (cvWaitKey(33)==27)
         break;
   }


   cvSetImageROI(img, cvRect(Pt1.x,Pt1.y,abs(Pt2.x-Pt1.x),abs(Pt2.y-Pt1.y)));
   cvSaveImage ("simple1.jpg", img, 0);
   cvShowImage("Template",img);
}


void buscar_patron_imagen(tTrackingVATS *descriptor){

   double minVal, maxVal;
   CvPoint minLoc;
   CvPoint maxLoc[descriptor->num_comp];

   double valormax=0;
   int numftmp=1;

   int j=1;
   int iwidth;
   int iheight;


   descriptor->Rsrc = cvCreateImage(cvGetSize(descriptor->src), IPL_DEPTH_8U, 1);
   cvSplit(descriptor->src, NULL, descriptor->Rsrc, NULL, NULL);

   while (j < descriptor->num_comp){

      iwidth = descriptor->Rsrc->width - Rtmp[j]->width + 1;
      iheight = descriptor->Rsrc->height - Rtmp[j]->height + 1;

      ftmp[j] = cvCreateImage(cvSize(iwidth, iheight), 32, 1);
      cvMatchTemplate(descriptor->Rsrc, Rtmp[j], ftmp[j], 5);
      cvMinMaxLoc(ftmp[j], &minVal, &maxVal, &minLoc, &maxLoc[j], NULL);
      
      if (valormax < maxVal){
         valormax = maxVal;
         //printf("\nValormax=%f, maxVal=%f, minVal=%f numftmp=%d\n", valormax, maxVal, minVal, j);
         numftmp = j;
      }
      j++;
   }

   if (valormax > 0.6){
      iwidth = descriptor->Rsrc->width - Rtmp[numftmp]->width + 1;
      iheight = descriptor->Rsrc->height - Rtmp[numftmp]->height + 1;
      ftmp[numftmp] = cvCreateImage(cvSize(iwidth, iheight), 32, 1);
      cvMatchTemplate(descriptor->Rsrc, Rtmp[numftmp], ftmp[numftmp], 5);
      cvNormalize( ftmp[numftmp], ftmp[numftmp], 1, 0, CV_MINMAX, NULL);
      cvPow(ftmp[numftmp],ftmp[numftmp],5);
      cvMinMaxLoc(ftmp[numftmp], &minVal, &maxVal, &minLoc, &maxLoc[numftmp], NULL);

      cvRectangle( descriptor->src, cvPoint(maxLoc[numftmp].x, maxLoc[numftmp].y), cvPoint(maxLoc[numftmp].x+Rtmp[numftmp]->width,maxLoc[numftmp].y+Rtmp[numftmp]->height), CV_RGB(255,0,0), 1, CV_AA, 0);
   }

   cvShowImage( "COEFF_NORMED", ftmp[numftmp]);
}

char VATS_Tracking_Start(tTrackingVATS *descriptor){
   descriptor->num_img = 0;
   descriptor->num_comp = 8;
   descriptor->drawing_box = 0;
   descriptor->num = 2;

   inicializar_imagenes_scaladas(descriptor);

   return '1';

}


char VATS_Tracking(tTrackingVATS *descriptor){
				  /*,
                                  IplImage *reference,
                                  IplImage *mask,
                                  CvRect region,
                                  IplImage *input,
                                  IplImage *inmask,
                                  CvRect output,
				  float *confidence
			         ){*/



   buscar_patron_imagen(descriptor);

   return '1';

}


void VATS_Tracking_Stop(tTrackingVATS *descriptor){

   int i = 1;
   while (i < descriptor->num_comp){
      cvReleaseImage (&Rtmp[i]);
      cvReleaseImage (&ftmp[i]);
      i++;
   }
   
   cvReleaseImage (&descriptor->Rsrc);

}

