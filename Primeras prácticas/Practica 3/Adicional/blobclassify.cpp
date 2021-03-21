#include "blobfuns.h"
#include "redn.h"
#include <iostream>

#include <opencv2/opencv.hpp>

int conthor[300], contver[300];
int contho[300], contve[300];
FILE *fp;

Red Clasif;

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
	
   IplImage *aux1, *aux2, *ImgBin;

   int width1, height1;
   float AR = 0;
   float areaRect, areaBlob;
   float compac = 0;
   CvPoint sP1, sP2;
   char auxTexto[8];

   int clase;

   BasicBlob *auxpblob;

   uchar *ppixelimg;	

   CvFont font;
   cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5);  
	
   //classify each blob of the list

   aux1 = frame;
   cvNamedWindow("Etiquetado",0);
   int nt=0, num_per=0, num_obj=0, num_des=0, num_aut=0, num_gru=0;
   int posX, posY, row, col;
   int j, k, contr, contv;
   int sumath, sumatv;
   float mediah = 0.0, mediav = 0.0, HIS = 0.0;
        
   j = 0;
   while (j < 700){
      conthor[j] = 0;
      contver[j] = 0;
      j++;
   }

   aux2 = cvCloneImage(frame);
   ImgBin = cvCreateImage(cvGetSize(aux2), aux2->depth, 1);
   cvCvtColor(aux2, ImgBin, CV_RGB2GRAY);
       
   for(int i = 0; i < pBlobList->getBlobNum(); i++) {
 
   if (Clasif.GetNum() == 0)
         if (Clasif.training_network() == -1){
            printf("No se pudo Entrenar la Red");
            return -1;
         }
      
      auxpblob = pBlobList->getBlob(i);
      posX = auxpblob->getX();
      posY = auxpblob->getY();
      width1 = auxpblob->getWidth();
      height1 = auxpblob->getHeight();

      AR = (float)width1/(float)height1;
      areaRect = width1*height1;
      areaBlob = auxpblob->getarea();

      compac = areaBlob/areaRect;

/*      for (row = posX; row <= posX+width1; row++) 
         for (col = posY; col <= posY+height1; col++)
         {
            ppixelimg = cvPtr2D( ImgBin, row, col, NULL );
            if (*ppixelimg == 255){
               conthor[row]++;
            }
            printf("%d \n", conthor[row]);
         }

      j = posX; k = 0;
      sumath = 0;
 
      while (j <= posX+width1)
      {
         if (conthor[j] != 0){
	    sumath = sumath + conthor[j];
            k++;
         }
	 j++;
      }

      if (k!=0)
         mediah = (float)sumath/(float)k;

      for (col = posY; col <= posY+height1; col++)
	 for (row = posX; row <= posX+width1; row++)
         {
            ppixelimg = cvPtr2D( ImgBin, row, col, NULL );
            if (*ppixelimg == 255){
               contver[col]++;
	    }
            printf("%d \n", contver[col]);	
         }

      j = posY; k = 0;
      sumatv = 0;
      while (j <= posY+height1)
      {
         if (contver[j] != 0){
	    sumatv = sumatv + contver[j];
	    k++;
	 }
	 j++;
      }
   
      if (k!=0)
         mediav = (float)sumatv/(float)k;
      printf("MediaV: %d %d %d %f %f \n", i, sumatv, k, mediah, mediav);

 
      if (mediah != 0)
         HIS = mediav/mediah;
      else
         HIS = 0;*/
      //Se tiene el problema de que no calcula los histogramas, indicado en el informe.
      

      sP1 = cvPoint(posX, posY);
      sP2 = cvPoint(posX + width1, posY + height1);

      fp = fopen("data1.txt", "w");
      fprintf(fp, "%f,%f\n", AR, compac);
      fclose(fp);

      clase = Clasif.test_network();

        
      if (clase == -1)
      {
         printf("Red Bad Training\n");
         return -1;
      }

      //Rangos obtenidos de las caracteristicas AR, compacidad e Histogramas obtenidos por medio de matlab 
      //Para utilizarlos descometelos de acuerdo a lo indicado en cada linea ver referencia numérica


      if (clase == 0){											//Comentar
      //if ((AR > 0.16) && (AR <= 0.50)){	//Media = 0,3509 Desviación = 0,0639 Estadisticas  	//1 							//obtenidas en matlab con un DataSet de 						//405 Imagenes
      //if ((compac > 0.39) && (compac <= 0.88)){							//2
      //if ((HIS > 0.296) && (HIS <= 0.7305)){								//3
         auxpblob->setlabel(PERSON);
         cvRectangle(aux1, sP1, sP2, CV_RGB(0,0,255), 1, CV_AA, 0);
         if (auxpblob->getlabel() == 1){
            strcpy(auxTexto, "PERSON"); 
            cvPutText(aux1, auxTexto, sP1, &font, CV_RGB(0,0,255));   	//AZUL PERSONAS
         }
         num_per++;
      }
      //else if ((AR > 0.87) && (AR <= 1.60)){								//1
      //else if ((compac > 0.39) && (compac <= 0.88)){							//2
      //else if ((HIS > 0.8094) && (HIS <= 1.2438)){							//3
           //auxpblob->setlabel(GROUP);									//Descomentar
           //cvRectangle(aux1, sP1, sP2, CV_RGB(0,255,0), 1, CV_AA, 0);					//Descomentar
           //if (auxpblob->getlabel() == 2)								//Descomentar
              //strcpy(auxTexto, "GROUP"); 								//Descomentar
              //cvPutText(aux1, auxTexto, sP1, &font, CV_RGB(0,255,0));	//VERDE GRUPO			//Descomentar
	  //}												//Descomentar
          //num_gru++;											//Descomentar
        //}												//Descomentar

        else if (clase == 2){										//Comentar        
        //else if ((AR > 0.50) && (AR <= 0.87)){							//1
	//else if ((compac > 0.46) && (compac <= 0.93)){						//2
        //else if ((HIS > 0.8868) && (HIS <= 1.5072)){							//3
           auxpblob->setlabel(OBJECT);
           cvRectangle(aux1, sP1, sP2, CV_RGB(255,0,255), 1, CV_AA, 0);
           if (auxpblob->getlabel() == 4){
              strcpy(auxTexto, "OBJECT"); 
              cvPutText(aux1, auxTexto, sP1, &font, CV_RGB(255,0,255));	//MORADO OBJETO
           }
           num_obj++;
        }
        
	else if (clase == 1){										//Comentar
        //else if ((AR > 1.60) && (AR <= 3.60)){							//1
	//else if ((compac = 0.32) && (compac <= 0,89)){						//2
        //else if ((HIS > 2.8247) && (HIS <= 5.2553)){							//3
           auxpblob->setlabel(CAR);
           cvRectangle(aux1, sP1, sP2, CV_RGB(255,255,0), 1, CV_AA, 0);
           if (auxpblob->getlabel() == 3){
              strcpy(auxTexto, "CAR"); 
              cvPutText(aux1, auxTexto, sP1, &font, CV_RGB(255,255,0));	//TOMATE CARROS
	   }
           num_aut++;
        }
        
        else{ 
           auxpblob->setlabel(UNKNOWN);
           cvRectangle(aux1, sP1, sP2, CV_RGB(255,128,128), 1, CV_AA, 0);
           if (auxpblob->getlabel() == 0)
              strcpy(auxTexto, "UNKNOWN");
              cvPutText(aux1, auxTexto, sP1, &font, CV_RGB(255,128,128)); //AMARILLO DESCONOCIDO
              num_des++;
           }
	   nt++;
	}

	cvShowImage("Etiquetado", aux1);

	//destroy all resources
	//...
	cvReleaseImage(&aux2);
        //cvReleaseImage(&ImgBin);
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
