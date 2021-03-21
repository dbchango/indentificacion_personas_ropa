#include <stdio.h>

#include "opencv2/core/core_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv/cv.h"

CvPoint Pt1, Pt2;
char *movie;

IplImage *Rtmp[10];
IplImage *ftmp[10];

typedef struct{
   int num_img;

   int num_comp;
   int drawing_box;
   int num;

   IplImage *src, *Rsrc;
   IplImage *tmp[10];

}tTrackingVATS;


void my_mouse_callback( int event, int x, int y, int flags, void* param );

void detener_frame(char *movie, tTrackingVATS *descriptor);

void captura_patron_imagen(tTrackingVATS *descriptor);

void scalar_imagenes();

void buscar_patron_imagen(tTrackingVATS *descriptor);


char VATS_Tracking_Start(tTrackingVATS *descriptor);

void VATS_Tracking_Stop(tTrackingVATS *descriptor);

char VATS_Tracking(tTrackingVATS *descriptor);
				  /*,
                                  IplImage *reference,
                                  IplImage *mask,
                                  CvRect region,
                                  IplImage *input,
                                  IplImage *inmask,
                                  CvRect output,
				  float *confidence
			         );*/



