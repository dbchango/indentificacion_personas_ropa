#include "blobfuns.h"
#include <opencv2/opencv.hpp>

/**
 *	Blob classificaion between the five available classes in 'BasicBlob.h' (see CLASS typedef). All the input arguments must be 
 *  initialized when using this function.
 *
 * \param frame Input image
 * \param fgmask Foreground/Background segmentation mask (1-channel) 
 * \param pBlobList List to store the blobs found 
 *
 * \return Operation code (negative if not succesfull operation) 
 */
int classifyBlobs(IplImage* frame, IplImage *fgmask, BlobList *pBlobList)
{
	//check input conditions and return -1 if any is not satisfied
	//...		
	if ((!frame) || (!fgmask))
           return -1;	

	//required variables for classification
	//...
	
	IplImage *aux1;

	float width1, height1;
	float AR, areaRect, areaBlob, compac;
	CvPoint sP1, sP2;
	char auxTexto[8];

        BasicBlob *auxpblob;
	
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5);  
	
	//classify each blob of the list

	aux1 = frame;
        cvNamedWindow("Etiquetado",0);
        int nt=0, num_per=0, num_obj=0, num_des=0, num_aut=0, num_gru=0;

	for(int i = 0; i < pBlobList->getBlobNum(); i++) {

           auxpblob = pBlobList->getBlob(i);
 
           width1 = auxpblob->getWidth();
	   height1 = auxpblob->getHeight();

           AR = width1/height1;
           areaRect = width1*height1;
           areaBlob = auxpblob->getarea();

	   compac = areaBlob/areaRect;
           //printf("AREAR = %d  AREAB = ");

           sP1 = cvPoint(auxpblob->getX(), auxpblob->getY());
           sP2 = cvPoint(auxpblob->getX() + auxpblob->getWidth(), auxpblob->getY() + auxpblob->getHeight());


           if ((AR > 0.16) && (AR <= 0.50)){	//Media = 0,3509 Desviación = 0,0639 Estadisticas Optenidas en matlab con un DataSet de 						//405 Imagenes
           //   if ((compac > 0.39) && (compac <= 0.88)){
                 auxpblob->setlabel(PERSON);
                 cvRectangle(aux1, sP1, sP2, CV_RGB(0,0,255), 1, CV_AA, 0);
                 if (auxpblob->getlabel() == 1)
                     strcpy(auxTexto, "PERSON"); 
                 cvPutText(aux1, auxTexto, sP1, &font, CV_RGB(0,0,255));   	//AZUL PERSONAS
                 num_per++;
              //}

           }
           else if ((AR > 0.87) && (AR <= 1.60)){
	   //   else if ((compac > 0.39) && (compac <= 0.88)){
                 auxpblob->setlabel(GROUP);
                 cvRectangle(aux1, sP1, sP2, CV_RGB(0,255,0), 1, CV_AA, 0);
                 if (auxpblob->getlabel() == 2)
                    strcpy(auxTexto, "GROUP"); 
                 cvPutText(aux1, auxTexto, sP1, &font, CV_RGB(0,255,0));	//VERDE GRUPO
                 num_gru++;
	      //}
           }
           else if ((AR > 0.50) && (AR <= 0.87)){
	   //   else if ((compac > 0.46) && (compac <= 0.93)){
                 auxpblob->setlabel(OBJECT);
                 cvRectangle(aux1, sP1, sP2, CV_RGB(255,0,255), 1, CV_AA, 0);
                 if (auxpblob->getlabel() == 3)
                    strcpy(auxTexto, "OBJECT"); 
                 cvPutText(aux1, auxTexto, sP1, &font, CV_RGB(255,0,255));	//MORADO OBJETO
                 num_obj++;
	       //}

           }
           else if ((AR > 1.60) && (AR <= 3.60)){
	   //   else if ((compac = 0.32) && (compac <= 0,89)){
                 auxpblob->setlabel(CAR);
                 cvRectangle(aux1, sP1, sP2, CV_RGB(255,255,0), 1, CV_AA, 0);
                 if (auxpblob->getlabel() == 4)
                    strcpy(auxTexto, "CAR"); 
                 cvPutText(aux1, auxTexto, sP1, &font, CV_RGB(255,255,0));	//TOMATE CARROS
	      //}
              num_aut++;
           }
           else{ 
              auxpblob->setlabel(UNKNOWN);
              cvRectangle(aux1, sP1, sP2, CV_RGB(255,128,128), 1, CV_AA, 0);
              if (auxpblob->getlabel() == 0)
                 strcpy(auxTexto, "UNKNOWN");
              cvPutText(aux1, auxTexto, sP1, &font, CV_RGB(255,128,128));	//AMARILLO DESCONOCIDO
              num_des++;
           }
	   nt++;
printf(" %d %d %d %d %d %d \n", nt, num_per, num_gru, num_obj, num_aut, num_des);
	}

	cvShowImage("Etiquetado", aux1);

	//destroy all resources
	//...
	//cvReleaseImage(&aux1);
	
	//return OK code
	return 1;
}


/**
 *	Draw blobs (and its classes) with different color rectangles on the image 'frame'. All the input arguments must be 
 *  initialized when using this function.
 *
 * \param frame Input image 
 * \param pBlobList List to store the blobs found 
 *
 * \return Image containing the draw blobs. If no blobs have to be painted 
 *  or arguments are wrong, the function returns NULL. The memory of this image 
 * is created inside this function so it has to be released after its use by the 
 * function calling 'paintBlobImage'.
 */
IplImage *paintBlobClasses(IplImage* frame, BlobList *pBlobList)
{
	//check input conditions and return NULL if any is not satisfied
	//...		

	//required variables to paint
	//...

	//paint each blob of the list
	for(int i = 0; i < pBlobList->getBlobNum(); i++) 	
	//...	

	//destroy all resources (if required)
	//...
	
	//return the image to show
	return frame; //blobImage;
}
