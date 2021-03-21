#include "vats_tracking.h"


int main(int argc, char** argv){

   if (argc==1){
      printf("Faltan argumentos: Nombre del Video?\n");
      exit(0);
   }
   movie = argv[1];

  
   tTrackingVATS desc;

   CvCapture *capture = NULL;
   capture = cvCaptureFromFile(movie);
   desc.num = 2;   

   detener_frame(movie, &desc);
   captura_patron_imagen(&desc);
   rescalar_imagenes(&desc);

   cvNamedWindow ("Seguimiento de Objeto", CV_WINDOW_AUTOSIZE);
   cvNamedWindow ("Template", CV_WINDOW_AUTOSIZE);
   cvNamedWindow ("COEFF_NORMED", CV_WINDOW_AUTOSIZE);


   if (VATS_Tracking_Start(&desc) != '1'){
      printf("Programa mal Inicializado");
      exit(0);
   }
 
   int i=1;

   while (1){

      desc.src = cvQueryFrame(capture);

      if (!desc.src)
         break;

      if (i >= desc.num_img)
          if (VATS_Tracking(&desc) != '1')  
              exit(1);                      

      cvShowImage ("Seguimiento de Objeto", desc.src);

      i++;
      if (cvWaitKey (100)==27)
         break;

   }


   VATS_Tracking_Stop(&desc);

   cvDestroyWindow ("Seguimiento de Objeto");
   cvDestroyWindow ("Template");
   cvDestroyWindow ("COEFF_NORMED");

   cvReleaseCapture(&capture);

}

