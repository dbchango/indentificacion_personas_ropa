#include "decvarseg.h"

char VATS_Foreground_Segmentation_Start(tForegroundSegmentationVATS *descriptor){
//Inicializa las variables del puntero a la estructura tForegroundSegmentationVATS

   //==========   Inicio en Caliente   ==========//
   //descriptor->ImgFonGris = cvCloneImage(descriptor->ImgFrist);
   descriptor->ImgFonGris = cvCreateImage(cvGetSize(descriptor->ImgFrist),descriptor->ImgFrist->depth,1);
   cvCvtColor(descriptor->ImgFrist, descriptor->ImgFonGris, CV_RGB2GRAY);

   //Copia la Primera Imagen en la Imagen de Fondo
   
   descriptor->ImgNueGris = cvCreateImage(cvGetSize(descriptor->ImgFonGris),descriptor->ImgFonGris->depth,1);
   //Separa espacio de memoria para la Nueva Imagen basada en la Imagen del Fondo 1 solo canal
   descriptor->ImgFreGris = cvCreateImage(cvGetSize(descriptor->ImgFonGris),descriptor->ImgFonGris->depth,1);
   //Separa espacio de memoria para el Frente basada en la Imagen del Fondo 1 solo canal

   descriptor->ImgLocked = cvCreateImage(cvGetSize(descriptor->ImgFrist),descriptor->ImgFrist->depth,1);
   //Separa espacio de memoria para el Locked basada en la Imagen del Fondo 1 solo canal

   descriptor->width = descriptor->ImgFonGris->width;
   descriptor->height = descriptor->ImgFonGris->height;
   //Obtiene el ancho y largo de la Imagen del Fondo

   int row, col;	//Variables para manejar pixeles, filas y columnas
   uchar *ppixelImgLoc;	//Variable para manipular contenidos de pixeles
   
   for (row = 0; row < descriptor->height; row++)
      for (col = 0; col < descriptor->width; col++){
          descriptor->ConLockedFon[row][col]=0;
          descriptor->ConLockedFre[row][col]=0;
          ppixelImgLoc = cvPtr2D( descriptor->ImgLocked, row, col, NULL );
          *ppixelImgLoc = 0;
      }
   //Setea el candado y contadores a cero

   //==========   Sombras   ==========//

   //Separa espacio de memoria para el Fondo basada en la Primera Imagen 1 solo canal
   descriptor->ImgFonGris1 = cvCreateImage(cvGetSize(descriptor->ImgFrist),descriptor->ImgFrist->depth,1);

   //Separa espacio de meoria para 
   //Imagen Fondo: HSV, HSV 32F, Hue 32F, Saturacion 32F, Value 32F
   descriptor->ImgFonHSV = cvCreateImage(cvGetSize(descriptor->ImgFrist),descriptor->ImgFrist->depth,3);
   descriptor->ImgFonHSV32F = cvCreateImage(cvGetSize(descriptor->ImgFrist),IPL_DEPTH_32F,3);
   descriptor->ImgFonHue32F = cvCreateImage(cvGetSize(descriptor->ImgFrist),IPL_DEPTH_32F,1);
   descriptor->ImgFonSal32F = cvCreateImage(cvGetSize(descriptor->ImgFrist),IPL_DEPTH_32F,1);
   descriptor->ImgFonVal32F = cvCreateImage(cvGetSize(descriptor->ImgFrist),IPL_DEPTH_32F,1);
   
   cvCvtColor(descriptor->ImgFrist, descriptor->ImgFonGris1, CV_RGB2GRAY);

   //Cambia a Imagen HSV la imagenRGV
   cvCvtColor(descriptor->ImgFrist, descriptor->ImgFonHSV, CV_RGB2HSV);

   //Convierte la imagen HSV de 8 a 32F
   cvConvertScale(descriptor->ImgFonHSV, descriptor->ImgFonHSV32F, 1.0, 0.0);

   //Divide la Imgaen HSV a sus correspondientes Hue, Saturación y Value de 32F
   cvSplit(descriptor->ImgFonHSV32F, descriptor->ImgFonHue32F, descriptor->ImgFonSal32F, descriptor->ImgFonVal32F, 0);

   descriptor->ImgNueGris1 = cvCreateImage(cvGetSize(descriptor->ImgFonGris1),descriptor->ImgFonGris1->depth,1);

   //Separa espacio de meoria para 
   //Imagen Nueva: HSV, HSV 32F, Hue 32F, Saturacion 32F, Value 32F
   descriptor->ImgNueHSV = cvCreateImage(cvGetSize(descriptor->ImgFrist),descriptor->ImgFrist->depth,3);
   descriptor->ImgNueHSV32F = cvCreateImage(cvGetSize(descriptor->ImgFrist),IPL_DEPTH_32F,3);
   descriptor->ImgNueHue32F = cvCreateImage(cvGetSize(descriptor->ImgFrist),IPL_DEPTH_32F,1);
   descriptor->ImgNueSal32F = cvCreateImage(cvGetSize(descriptor->ImgFrist),IPL_DEPTH_32F,1);
   descriptor->ImgNueVal32F = cvCreateImage(cvGetSize(descriptor->ImgFrist),IPL_DEPTH_32F,1);

   //Separa espacio de meoria para de frente de sombras
   descriptor->ImgFreGris1 = cvCreateImage(cvGetSize(descriptor->ImgFonGris1),descriptor->ImgFonGris1->depth,1);

   //Separa espacio de meoria para 
   //Variables auxiliares para el tratamiento de sombras.
   descriptor->divisionV32F = cvCreateImage(cvGetSize(descriptor->ImgFonGris1),IPL_DEPTH_32F,1);
   descriptor->restaS32F = cvCreateImage(cvGetSize(descriptor->ImgFonGris1),IPL_DEPTH_32F,1);
   descriptor->restaH132F = cvCreateImage(cvGetSize(descriptor->ImgFonGris1),IPL_DEPTH_32F,1);
   descriptor->restaH232F = cvCreateImage(cvGetSize(descriptor->ImgFonGris1),IPL_DEPTH_32F,1);
   descriptor->minH32F = cvCreateImage(cvGetSize(descriptor->ImgFonGris1),IPL_DEPTH_32F,1);

   //Copia la Primera Imagen del Fondo como Frente Resultante
   descriptor->ImgFreGrisR = cvCloneImage(descriptor->ImgFonGris1);

   return 255;
}


IplImage *Fsombyref(tForegroundSegmentationVATS *descriptor){
//Funciónque permite encontrar las sombras en la secuencia de video
//utilizando la técnica de Cucchiara

   CvScalar escalar={360};  
   //Declaración de variable tipo escalar para tecnica

   cvDiv(descriptor->ImgNueVal32F,descriptor->ImgFonVal32F, descriptor->divisionV32F, 1.0);
   //División entre Value de Nueva Imagen y value de Imagen de Fondo

   cvAbsDiff(descriptor->ImgNueSal32F, descriptor->ImgFonSal32F, descriptor->restaS32F);
   //valor absoluto de la diferencia entre valores de Saturación de la Imagen Nueva e Imagen de Fondo

   cvAbsDiff(descriptor->ImgNueHue32F, descriptor->ImgFonHue32F, descriptor->restaH132F);
   //valor absoluto de la diferencia entre valores de Hue de la Imagen Nueva e Imagen de Fondo

   cvAbsDiffS(descriptor->restaH132F, descriptor->restaH232F, escalar);
   //valor absoluto de la diferencia entre la resta de Hue123F y 360 - Hue32F

   cvMin(descriptor->restaH132F, descriptor->restaH232F, descriptor->minH32F);
   //minimo valor entre Hues

   int row,col;

   double ppixeldivisionV32F;
   double ppixelrestaS32F;
   double ppixelminH32F;
   uchar *ppixelsombra;

   int ppnuegris, ppfongris;

   for (row=0; row<descriptor->ImgNueHSV->height; row++)		
      for(col=0; col<descriptor->ImgNueHSV->width; col++){		

         ppixeldivisionV32F = cvGetReal2D(descriptor->divisionV32F,row,col);
         ppixelrestaS32F = cvGetReal2D(descriptor->restaS32F,row,col);
         ppixelminH32F = cvGetReal2D(descriptor->minH32F,row,col);
         ppixelsombra = cvPtr2D(descriptor->ImgFreGris1,row,col,NULL); 
         /*los ppixel les cambio de uchar * a double y la funcion cvPrt2D a cvGetReal2D*/


         if (((ppixeldivisionV32F <= descriptor->beta) && (ppixeldivisionV32F >= descriptor->alfaSomb)) &&
            (ppixelrestaS32F <= descriptor->umbralS) && 
             (ppixelminH32F <= descriptor->umbralH)){
            //Si cumple los rangos establecido por cucchiara sobre Hue, Saturacion y Hue, el pixel es 255
            *ppixelsombra = 128;
         }
         else
            //Caso contrario el pixel es 0
            *ppixelsombra = 0;
      }

   return (descriptor->ImgFreGris1);
   //Devuelva la Imagen resultante de sombras
}


void MInicioCaliente(tForegroundSegmentationVATS *descriptor){
//Funciónque permite encontrar el fonfo y frente de la secuencia de video
//utilizando el algoritmo de Candados   

   //Declaración de variables fila y columnas
   //Declaración de variables tipo pixeles para manuplar la imagen por pixeles
   int row, col;
   uchar *ppixelImgFreGris;
   uchar *ppixelImgFonGris;
   uchar *ppixelImgNueGris;
   uchar *ppixelImgLoc;

   //Barre todos los pixeles de la Imagen
   for (row = 0; row < descriptor->height; row++)
      for (col = 0; col < descriptor->width; col++)
      {
         ppixelImgFonGris = cvPtr2D( descriptor->ImgFonGris, row, col, NULL );  //Puntero a pixel de la Imagen del Fondo
         ppixelImgNueGris = cvPtr2D( descriptor->ImgNueGris, row, col, NULL );  //Puntero a pixel de la Imagen Nueva
         ppixelImgFreGris = cvPtr2D( descriptor->ImgFreGris, row, col, NULL );  //Puntero a pixel de la Imagen del Frente
         ppixelImgLoc = cvPtr2D( descriptor->ImgLocked, row, col, NULL  );	//Puntero a pixel de la Imagen Candado

         if (abs((*ppixelImgFonGris)-(*ppixelImgNueGris)) < descriptor->umbral){
	 //Si la resta de la Imagen delfondo con la Nueva Imagen es menor que un umbral este es frente
            *ppixelImgFreGris = 0;
            descriptor->ConLockedFre[row][col]=0;	//Contador de fantasmas ponga a 0;
            descriptor->ConLockedFon[row][col]++;	//Contador de Inicio en Caliente
	       
	    if (descriptor->ConLockedFon[row][col] > descriptor->umbic){
	    //Si Contador de Inicio en Caliente es mayor a umbral el pixel ponga 255 (frente)
               *ppixelImgLoc = 255;
               descriptor->ConLockedFon[row][col]=0;
            }
            else
	    //Caso contrario es fondo
               *ppixelImgLoc = 0;
	       
	    if (*ppixelImgLoc == 255){
	    //Si Imagen de candado es 255 realice adaptación selectiva (promedio moviles) con Imagen Nueva
               *ppixelImgFonGris = descriptor->alfa*(*ppixelImgNueGris) + (1 - descriptor->alfa)*(*ppixelImgFonGris);
            }
            else
	    //Caso contrario el Fondo es la Imagen Nueva
               *ppixelImgFonGris = *ppixelImgNueGris;
         }

         else{
	 //Caso contraio ponga el pixel a 255 (Frente)
            *ppixelImgFreGris = 255;
            descriptor->ConLockedFon[row][col]=0;	//Contador de Inicio en Caliente ponga a cero
            descriptor->ConLockedFre[row][col]++;	//Contador de fantasmas sume 1

            if (descriptor->ConLockedFre[row][col] > descriptor->umbsf){   
            //Si contador de fantasmas es mayor que umbral de fantasmas ponga contador Lock Frente a 0
            //   *ppixelImgLoc = 0;
               descriptor->ConLockedFre[row][col]=0;
            }
            else
	    //Caso contario ponga Locked a 255
               *ppixelImgLoc = 255;

            if (*ppixelImgLoc == 0)
            //Si locked es 0 Imagen del Fondo es la Imagen Nueva
               *ppixelImgFonGris = *ppixelImgNueGris;
            else
	    //Si Imagen de candado es 255 realice adaptación selectiva (promedio moviles) con Imagen Nueva
               *ppixelImgFonGris = (1 - descriptor->alfa)*(*ppixelImgFonGris) + descriptor->alfa*(*ppixelImgNueGris);
         }
      }
    
}


IplImage *fusion_inicaliente_sombras(tForegroundSegmentationVATS *descriptor){
//Función que permite encontrar el frente sin fantasmas, sombras e Incio en caliente 

   //Declaración de variables fila y columnas
   //Declaración de variables tipo pixeles para manuplar la imagen por pixeles   
   int row, col;
   uchar *ppixelImgFreGris;
   uchar *ppixelImgFreGris1;
   uchar *ppixelImgFreGrisR;

   //Barre todos los pixeles de la Imagen
   for (row = 0; row < descriptor->ImgFreGris->height; row++)
      for (col = 0; col < descriptor->ImgFreGris->width; col++)
      {
         ppixelImgFreGris = cvPtr2D( descriptor->ImgFreGris, row, col, NULL );	//Puntero a pixel de la Imagen del Frente
         ppixelImgFreGris1 = cvPtr2D( descriptor->ImgFreGris1, row, col, NULL );//Puntero a pixel de la Imagen del Frente 1
         ppixelImgFreGrisR = cvPtr2D( descriptor->ImgFreGrisR, row, col, NULL );//Puntero a pixel de la Imagen del Frente Resultante

	
         if ((*ppixelImgFreGris == 255) && (*ppixelImgFreGris1 == 0))
         //Si la Imagen no tiene sombras ponga a 255
            *ppixelImgFreGrisR = 255;
         else if (*ppixelImgFreGris1 == 128)
         //Caso contrario si la Imagen tiene sombras ponga a 0 la Imagen Resultante
            *ppixelImgFreGrisR = 128;
         else 
         //Caso contrario ponga el pixel del frente en la imagen Resultante
            *ppixelImgFreGrisR = *ppixelImgFreGris;
      }

   return descriptor->ImgFreGrisR;
   //Devuelva la Imagen resultante sin sombras ni fantasmas,Inicio en Caliente
}


char VATS_Foreground_Segmentation( tForegroundSegmentationVATS *descriptor,
				   IplImage *input,
				   char shadows,
				   IplImage *mask
				 ){   

// INICIO EN CALIENTE
   cvCvtColor(input, descriptor->ImgNueGris, CV_RGB2GRAY);
   MInicioCaliente(descriptor);
   mask = descriptor->ImgFreGris;

// SOMBRAS
   if (shadows == '1'){
      cvCvtColor(input, descriptor->ImgNueGris1, CV_RGB2GRAY);
      cvCvtColor(input, descriptor->ImgNueHSV, CV_RGB2HSV);
      cvConvertScale(descriptor->ImgNueHSV, descriptor->ImgNueHSV32F, 1.0, 0.0);
      cvSplit(descriptor->ImgNueHSV32F, descriptor->ImgNueHue32F, descriptor->ImgNueSal32F, descriptor->ImgNueVal32F, 0);   
      mask = Fsombyref(descriptor);
   }

// INICIO EN CALIENTE Y SOMBRAS
   mask = fusion_inicaliente_sombras(descriptor);

   return 255;
}

void VATS_Foreground_Segmentation_Stop(tForegroundSegmentationVATS *descriptor){
//Permite liberar espacio de memoria de los espacios de memoria obtenidas del descriptor

   //Libera espacio de memoria de

   cvReleaseImage(&descriptor->ImgFrist);	//Primera Imagen

   //Fondo binario, Nueva binario, frente Binario, Locked (Inicio en Caliente)
   cvReleaseImage(&descriptor->ImgFonGris);
   cvReleaseImage(&descriptor->ImgNueGris);
   cvReleaseImage(&descriptor->ImgFreGris);
   cvReleaseImage(&descriptor->ImgLocked);

   //Fondo binario, Nueva binario, frente Binario (Sombras)
   cvReleaseImage(&descriptor->ImgFonGris1);	
   cvReleaseImage(&descriptor->ImgNueGris1);
   cvReleaseImage(&descriptor->ImgFreGris1);

   //Fondo HSV y Nueva HSV
   cvReleaseImage(&descriptor->ImgFonHSV);
   cvReleaseImage(&descriptor->ImgNueHSV);

   //Fondo Hue, Fondo Saturacion, Fondo Value
   cvReleaseImage(&descriptor->ImgFonHue32F);
   cvReleaseImage(&descriptor->ImgFonSal32F);
   cvReleaseImage(&descriptor->ImgFonVal32F);

   //Nueva Hue, Nueva Saturacion, Nueva Value
   cvReleaseImage(&descriptor->ImgNueHue32F);
   cvReleaseImage(&descriptor->ImgNueSal32F);
   cvReleaseImage(&descriptor->ImgNueVal32F);

   //Variables auxiliares para determinar los umbrales de las sombras
   cvReleaseImage(&descriptor->divisionV32F);
   cvReleaseImage(&descriptor->restaS32F);
   cvReleaseImage(&descriptor->restaH132F);
   cvReleaseImage(&descriptor->restaH232F);
   cvReleaseImage(&descriptor->minH32F);

   //Imagen Resultante de sombras
   cvReleaseImage(&descriptor->ImgFreGrisR);

}


