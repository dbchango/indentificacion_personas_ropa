//Archivo de cabecera de la Red Neuronal
//Definición de la clase Red  

#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdlib.h>

#include "opencv/cv.h"
#include "opencv/ml.h"

/******************************************************************************/
//Definiciones globales

#define NUMBER_OF_TRAINING_SAMPLES 440
#define ATTRIBUTES_PER_SAMPLE 2
#define NUMBER_OF_TESTING_SAMPLES 1

//El número de classes a clasificar estan en el rango de 0-2
#define NUMBER_OF_CLASSES 3


/******************************************************************************/

//Carga los ejemplos del archivo ent.txt y data1.txt

using namespace cv; // OpenCV API para C++ "cv" namespace


//Clase que implementa el Clasificador Neuronal tipo backPropagation
class Red{
     int num;			//Variable que permite controlar la ejecución de la función training:network, se entrena una sola vez la Red
     CvANN_MLP* nnetwork;	//Declaración de la Red Neuronal
  public:
     //Constructor de la clase Red, setea num a 0
     Red(){num = 0;};
     //Leer datos de entrenamiento y de clasificación
     int read_data_from_txt(const char* filename, Mat data, Mat classes, int n_samples );
     //Función de Entrenamiento de la Red en base a las caracteristicas Aspect Radio y Compacidad
     int training_network();
     //Función que permite clasificar los datos obtenidos de las imágenes: Aspect Radio y Compacidad
     int test_network();
     //Función que inserta un valor a num
     void SetNum(int n){num = n;};
     //Función que retorna el valor de num
     int GetNum(){return num;};
};
