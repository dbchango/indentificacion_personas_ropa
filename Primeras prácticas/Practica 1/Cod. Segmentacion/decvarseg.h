//Archivo de definición de 


//Se importa la libreria de entrada y salida de datos por consola
#include <stdio.h>

//Se incluye las librerias para el tratamiento de imagenes
#include "opencv2/core/core_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui_c.h"

//Se define el archivo de video a procesar
#define mivideo "mh.mpg"
//#define mivideo "hall2.mpg"
//#define mivideo "acc.mpg"
//#define mivideo "A.avi"
//#define mivideo "vga.mpg"

//Definición de la estructura: tForegroundSegmentationVATS

typedef struct{
   
   //Declaración de variable tipo Imagen para el tratamiento de imagenes (inicio en caliente)
   //(Primera Imagen, Fondo, Nueva Imagen, Máscara de movimiento (Frente), Mascada Locked, Primera Imagen en 3 canales
   IplImage *ImgFrist, *ImgFonGris, *ImgNueGris, *ImgFreGris, *ImgLocked, *ImgFrist3;

   //Declaración de Variables tipo Imagen para el tratamiento de sombras:
   //(Fondo, máscara de sombras (Frente), Nueva Imagen y máscara resultante
   IplImage *ImgFonGris1, *ImgFreGris1, *ImgNueGris1, *ImgFreGrisR;

   int umbral;		//Umbral para determinar si es fondo
   double alfa;		//Umbral para determinar si es fantasma
     
   int width, height;	//ancho y alto de una imagen
   int umbic, umbsf;	//Umbrales de inicio en caliente y fantasmas

   int ConLockedFon[720][400];	//Contador de candado que permite asegurar que es fondo 
   int ConLockedFre[720][400];	//Contador de candado que permite asegurar que es frente

   //Declaración de Variables tipo Imagen para el tratamiento de sombras
   //Utilizando la Técnica de Cucchiara
   //Imagen Fondo: HSV, HSV 32F, Hue 32F, Saturacion 32F, Value 32F
   //Imagen Nueva: HSV, HSV 32F, Hue 32F, Saturacion 32F, Value 32F 
   
   IplImage *ImgNueHSV, *ImgNueHSV32F, *ImgNueHue32F, *ImgNueSal32F, *ImgNueVal32F;
   IplImage *ImgFonHSV, *ImgFonHSV32F, *ImgFonHue32F, *ImgFonSal32F, *ImgFonVal32F;
   //Variables auxiliares
   IplImage *divisionV32F, *restaS32F, *restaH132F, *restaH232F, *minH32F;

   //Declaración de umbrales de sombras: Alfa, beta, Saturación, Hue
   double alfaSomb, beta, umbralS;
   int umbralH;

}tForegroundSegmentationVATS;

//==========   Declaración de funciones para el procesamiento de imagenes   ==========//

char VATS_Foreground_Segmentation_Start(tForegroundSegmentationVATS *descriptor);
//Permite inicializar partes del puntero a la estructura tForegroundSegmentationVATS descriptor


void VATS_Foreground_Segmentation_Stop(tForegroundSegmentationVATS *descriptor);
//Permite liberar espacio de memoria de las variables creadas

char VATS_Foreground_Segmentation(tForegroundSegmentationVATS *descriptor,
                                  IplImage *input,
				  char shadows,
				  IplImage *mask
			         );
//Permite realizar el tratamiento de las imagenes de video
