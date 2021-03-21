//Implementación del Clasificador de Blob a traves de Redes Neuronales: BackPropagation
//Implementación de la Clase Red

#include "redn.h"

/******************************************************************************/

//Lee los datos de entrenamiento y de clasificación de los archivos planos ent.txt y val.txt
//filename: ent.txt Datos de entrenamiento (Datos caracteristicos extraidos: AR y Compacidad)
//filename: val.txt Datos de validación (Datos caracteristicos extraidos de cada imagen procesada)
//data y clases separa los atributos característicos de un objeto del tipo de objeto 

int Red::read_data_from_txt(const char* filename, Mat data, Mat classes, int n_samples)
{

   int classlabel; //Etiqueta de clase
   float tmpf;
   int fs;

   //Apertura del archivo filename para leer datos de entrenamientoy validación
   //Si no se puede abrir retorna 0
   FILE* f = fopen( filename, "r" );
   if( !f )
   {
      printf("ERROR: cannot read file %s\n",  filename);
      return 0; // all not OK
   }

   //Por cada ejemplo del archivo filename procesa 
   for(int line = 0; line < n_samples; line++)
   {
      //Por cada atributo de la linea del archivo
      for(int attribute = 0; attribute < (ATTRIBUTES_PER_SAMPLE + 1); attribute++)
      {
         //Si el atributo es menor al numero de caracteristicas (AR, Compacidad)
         if (attribute < ATTRIBUTES_PER_SAMPLE)
         {
            //Leemos el valor del primer atributo de la línea, luego el segundo (0-1) 
            fscanf(f, "%f,", &tmpf);
	    //Insertamos en la variable tipo Mat (data), en la posición linea, atributo el valor del atributo leido tmpf
            data.at<float>(line, attribute) = tmpf;
         }
         //Si el atributo es igual al numero de caracteristicas (clase)
         else if (attribute == ATTRIBUTES_PER_SAMPLE)
         {
            //Leemos el tercer atributo que identifica la etiqueta de la clase {Persona, auto, maleta/objeto}
            fscanf(f, "%i", &classlabel);
            //Insertamos en la variable tipo Mat (clases), en la posición linea, classlabel el valor de 1
            classes.at<float>(line, classlabel) = 1.0;
         }
      }
   }

   //Cerramos el archivo filename
   fclose(f);

   return 1; //Si todo esta OK
}


/******************************************************************************/

//Función de Entrenamiento de la Red en base a las caracteristicas Aspect Radio y Compacidad
int Red::training_network()
{

   //Declaración y definición de las matrices para el entrenamiento, datos y clases

   Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
   Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES, NUMBER_OF_CLASSES, CV_32FC1);

   //Inicializa la red neuronal
   nnetwork = new CvANN_MLP;

   //Lee y carga los datos de entrenamiento en las matrices de datos y clases de acuerdo al número de ejemplos de entrenamiento

   if (read_data_from_txt("ent.txt", training_data, training_classifications, NUMBER_OF_TRAINING_SAMPLES))
   {
      //Define el numero de parametros de la red neuronal (MLP)

      //Setea la red: 3 capas, un nodo por atributo por atributo, nodos ocultos, un nodo de salida por clase

      //La implementación de redes neuronales en OpenCV (MLP) no soportan variables explicitas 
      //Para etiquetar la salida (clases) de la red selas realiza mediante un vector binario {0,0 ... 1,0,0} componentes
      //En la fase deentrenamiento, MLP obtiene un vector de "probabilidades"
      //En la fase predicción la mas alta probabilidad será aceptada como la clase ganadora y sera la salida de la Red

      //Declaración de las capas 3 capas
      int layers_d[] = { ATTRIBUTES_PER_SAMPLE, 10,  NUMBER_OF_CLASSES};
      Mat layers = Mat(1,3,CV_32SC1);
      layers.at<int>(0,0) = layers_d[0];
      layers.at<int>(0,1) = layers_d[1];
      layers.at<int>(0,2) = layers_d[2];

      //Creamos la Red utilizando una función sigmoide con un alpha and beta con valor de 0.6 y 1 valores por default (Manual de 
      //Referencias)
      
      nnetwork->create(layers, CvANN_MLP::SIGMOID_SYM, 0.6, 1);

      //Seteo los parametros de entrenamiento:
      //Realiza un maximo de 1000 iteraciones o un valor pequeño de cambio de los pesos sinapticos de 0.000001
      //CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,
      //Utiliza la red Backpropagation para el entrenamiento con los coeficientes por default (Manual de Referencias)
      //http://docs.opencv.org/modules/ml/doc/neural_networks.html
      CvANN_MLP_TrainParams params = CvANN_MLP_TrainParams( cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 0.000001),
                                    CvANN_MLP_TrainParams::BACKPROP, 0.1, 0.1);

      //Entrena la red neuronal usando los datos de entrenamiento y las clase definidas en ent.txt
      int iterations = nnetwork->train(training_data, training_classifications, Mat(), Mat(), params);

      num = 1;  //Permite controlar que solo una vez se entrene la Red
      return 0; //Si todo esta Ok
   }

   return -1; //Si no esta Ok
}


/******************************************************************************/

//Función que permite clasificar los datos caracteristicos obtenidos de los blobs: Aspect Radio y Compacidad
int Red::test_network(){

   //Declaración y definición de las matrices para el entrenamiento, datos y clases

   Mat testing_data = Mat(NUMBER_OF_TESTING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
   Mat testing_classifications = Mat::zeros(NUMBER_OF_TESTING_SAMPLES, NUMBER_OF_CLASSES, CV_32FC1);

   //Declaración y definición de las matriz de clasificación de la fase de entrenamiento

   Mat classificationResult = Mat(1, NUMBER_OF_CLASSES, CV_32FC1);
   Point max_loc = Point(0,0);

   //Lee y carga los datos caracteristicos obtenidos de los blobs en las matrices de validación de datos y clases de prueba

   if (read_data_from_txt("data1.txt", testing_data, testing_classifications, NUMBER_OF_TESTING_SAMPLES))
   {

      //Declara la matriz de validación y reporte de resultados
      Mat test_sample;

      //Extrae la fila obtenida de la matriz de validación datos de prueba
      test_sample = testing_data.row(0);

      //Llama a la función que predice los resultados, con la matriz de validación datos de prueba,
      //Se obtiene los resultados de la clasificación

      nnetwork->predict(test_sample, classificationResult);

      //La Red Neuronal obtiene un vector de probabilidades por cada clase, asignando a la clase con mayor probabilidad
      //como ganadora.
      //minMaxLoc: Fiunción que permite obtener el máximo valor de probabilidad
      minMaxLoc(classificationResult, 0, 0, 0, &max_loc);

      //Retorna la clase con mayor probabilidad
      return max_loc.x;
   }
   return -1;

}

/******************************************************************************/

