#include "blobfuns.h"
#include "opencv2/imgproc/imgproc_c.h"
#include <opencv2/opencv.hpp>

#define MIN_WIDTH 10
#define MIN_HEIGHT 10

/**
 *	Blob extraction from 1-channel image (binary). The extraction is performed based
 *	on the analysis of the connected components. All the input arguments must be 
 *  initialized when using this function.
 *
 * \param frame Input image
 * \param fgmask Foreground/Background segmentation mask (1-channel) 
 * \param pBlobList List to store the blobs found 
 *
 * \return Operation code (negative if not succesfull operation) 
 */
int extractBlobs(IplImage* frame, IplImage *fgmask, BlobList *pBlobList)
{	
	//check input conditions and return -1 if any is not satisfied
	//...	
	if ((!frame) || (!fgmask))
           return -1;	

	//required variables for connected component analysis 
	//...
        int comrecwidant, comrecheiant;

        int contador, valid;

	CvPoint seedPoint;
	CvConnectedComp comp;
        IplImage *aux = cvCloneImage(fgmask);

        BasicBlob *pblob;
      
	//clear blob list (to fill with this function)
	pBlobList->clear();

	//Connected component analysis
	//...
	CvScalar color = CV_RGB(128,128,128);
        CvScalar colorbmen = CV_RGB(200,200,200);
        CvScalar colorbmay = CV_RGB(255,255,255);
	
        uchar *ppixelfgmask;
	int row=0;
	int col=0;



//////////////////////////////////////////////////////

        aux = cvCloneImage(fgmask);
        comrecwidant =0;
        comrecheiant = 0;

        contador = 0;
        for (row=0;row<aux->height;row++) 
           for (col=0;col<aux->width;col++) {
              ppixelfgmask = cvPtr2D( aux, row, col, NULL );
              if (*ppixelfgmask == 255){
		 seedPoint = cvPoint(col,row);
        	 cvFloodFill(aux, seedPoint, color, colorbmen, colorbmay, &comp, 8, NULL);
              }		

	      //check size of blob using MIN_WIDTH & HEIGHT (valid = true)
	      //...
              if ((comp.rect.width > MIN_WIDTH) && (comp.rect.height > MIN_HEIGHT))
                 if ((comrecwidant != comp.rect.x) && (comrecheiant != comp.rect.y))
                    valid = 1;
                 else 
                    valid = 0;

	      //include blob in 'pBlobList' if it is valid
              
              if (valid == 1){
                 pblob = new BasicBlob();
                 pblob->setX(comp.rect.x);
                 pblob->setY(comp.rect.y);
                 pblob->setHeight(comp.rect.height);
                 pblob->setWidth(comp.rect.width);
                 pblob->setlabel(UNKNOWN);
                 pblob->setID(contador);
                 pblob->setarea(comp.area);

                 comrecwidant = comp.rect.x;
                 comrecheiant = comp.rect.y;
                 valid = 0;

                 pBlobList->addBlob(pblob);

   		 contador++;	 
              }	
           }


//////////////////////////////////////////////////////

//      	cvShowImage("FloodFill", aux);
	//destroy all resources
	//...
	cvReleaseImage(&aux);	

	//return OK code
	return 1;
}


/**
 *	Draw blobs with different rectangles on the image 'frame'. All the input arguments must be 
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
IplImage *paintBlobImage(IplImage* frame, BlobList *pBlobList)
{
	//check input conditions and return NULL if any is not satisfied
	//...		
	if (!frame)
           return frame;	
	//required variables to paint
	//...


	//paint each blob of the list
//for(int i = 0; i < pBlobList->getBlobNum(); i++) 	
	//...	



	//destroy all resources (if required)
	//...
	
	//return the image to show
	return frame; //blobImage;
}
