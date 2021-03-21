#include "decvarseg.h"

int main(){

   //Declara un descriptor tipo tForegroundSegmentationVATS
   //Contiene variables tipo imagen, contadores, umbrales, etc.
   tForegroundSegmentationVATS desc;
   //variable que retorna un 1 si las funciones se han ejecutado correctamente
   char c;
   CvCapture *CapVideo = NULL;			
   //Declaro una variable tipo CvCpture para capturar imagenes y realizar el tratamiento de las mismas
   
   CapVideo = cvCaptureFromFile(mivideo);
   //Captura el archivo de video

   if (!CapVideo){	//Verifica que existe el video, si no existe sale del programa
       printf("No se pudo abrir el archivo\n");	
       exit(-1);
   }

   IplImage *Imagen = NULL;
   //Declara la variable tipo Imagen, para ir obteniendo lasecuencia de imagenes del video 

   Imagen = cvQueryFrame(CapVideo);
   //obtiene la primera imagen del video

   //Se asigna el ancho y largo a la primera Imagen
   desc.width = 720;		
   desc.height = 400;

   //Inicialización de umbrales, para segmentación: incio en caliente, fantasmas y sombras
   desc.umbral = 30;		//variable que determina si es fondo o no

   desc.alfa = 0.05;		//Factor de aprendizaje (Adaptación selectiva)

   //Umbrales para determinar si es fondo (Inicio en Caliente)
   desc.umbic = 20;		//Asignación de valor al Umbral de Inicio en Caliente
   desc.umbsf = 20;		//Asignación de valor al Umbral de Fantasmas

   //Umbrales para determinar si un pixeles sombra
   desc.alfaSomb = 0.5;		//Asignación de valor al Umbral de Value (Rango Izquierdo)
   desc.beta = 0.75;		//Asignación de valor al Umbral de Value (Rango Derecho)
   desc.umbralS = 40; 		//Asignación de valor al Umbral de Saturación
   desc.umbralH = 36; 		//Asignación de valor al Umbral de Hue

   //Obtención de la primera imagen, que va a ser la Imagen de Fondo
   desc.ImgFrist = cvCreateImage(cvGetSize(Imagen), Imagen->depth, 1);	//Separa espacio de memoria para la Priemra Imagen en 1 solo canal
   desc.ImgFrist3 = cvCreateImage(cvGetSize(Imagen), Imagen->depth, 3);	//Separa espacio de memoria para la Priemra Imagen en 3 canales
   desc.ImgFrist = cvClone(Imagen);	//Copia la Primera Imagen de la Imgen obtenida anteriormente


   //==========   Inicialización de variables de imagenes del Descriptor   ==========//

   c = VATS_Foreground_Segmentation_Start(&desc);
   if (c==0){
      printf("No se pudo Inicializar");
      exit(1);
   }

   //Crea las Ventanas de la imagen original, máscara de movimiento, máscara de sombras y máscara resultante   
   cvNamedWindow("Imagen Original",0);		
   cvShowImage("Imagen Original", Imagen);
   cvNamedWindow("Mascara de Movimiento",0);
   cvNamedWindow("Mascara de Sombras",0);
   cvNamedWindow("Mascara Resultante",0);

   //Declaración de la Imagen Binaria Resultado y separación de espacio de memoria 
   IplImage *ImgFreBin;
   ImgFreBin = cvCreateImage(cvGetSize(Imagen), Imagen->depth, 1);

   //==========   Tratamiento de las imagenes de video   ==========//

   //Lazo que se ejecuta mientras exista una imagen
   while (1){

      //Vefica si existe una nueva imagen en el video
      if (!Imagen)
         break;
      //Llamada a función para el tratamiento de imagenes
      c=VATS_Foreground_Segmentation(&desc,Imagen,'1',ImgFreBin);
      if (c==0){
         printf("No existen mas Frame\n");
         break;
      }

      //Muestra las Imagenes: Imagen original, máscara de movimiento, máscara de sombras y máscara resultante
      cvShowImage("Imagen Original", Imagen);
      cvShowImage("Mascara de Movimiento", desc.ImgFreGris); 	//Imagen de Incio en Caliente
      cvShowImage("Mascara de Sombras", desc.ImgFreGris1);  	//Imagen de Sombras
      ImgFreBin = cvCloneImage(desc.ImgFreGrisR);	
      cvShowImage("Mascara Resultante", ImgFreBin);		//Imagen Resultanete

      //Espere 100 milisegundo para tratar otra imagen o pulse la tecla ESC para salir
      if (cvWaitKey(33)==27)
         break;

      //Obtiene nueva imagen
      Imagen = cvQueryFrame(CapVideo);

   }

   //Libera espacios de memoria utilizados, tanto del descriptor como de la Imagen Resultante y de la captura de Video
   VATS_Foreground_Segmentation_Stop(&desc);
   cvReleaseImage(&ImgFreBin);
   cvReleaseCapture(&CapVideo);

   return 0;
}
