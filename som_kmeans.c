#include "structures.h"



Liste * insert(Liste * winnersList, int x){ // add in the linkedlist, new is the new node, winnerlist is the list of all winners
    //printf("xxxxxxxxxxx == %d  yyyyyyyyyy == %d \n",x,y );
    Liste * new;

    new = (Liste*)malloc(sizeof(Liste));
    new->nombre = x;
    new->suivant = (Liste*)malloc(sizeof(Liste)); /////////////
    new->suivant = winnersList;
    winnersList = new;
    //printf("winnersList nombre %d \n",winnersList->nombre );

   // printf("winnersList function    %d  %d \n", winnersList->x, winnersList->y);
    return winnersList;
}



//retourne une nombre aléatoire entre a et b
double Randomdouble(double a, double b) {
    double diff = b - a;
    double random = RAND_MAX / diff;
    double r = rand() / random;
    return r+a;
}


int IndexListeChaine(Liste * element, int sizeList){
    int i=0,taille=0, WinnerNeurone;
    
    WinnerNeurone = Randomdouble((double)0,(double)sizeList);

    //printf("(element + neuroneGagnant)->nombre %d \n",(element + neuroneGagnant)->nombre );
  return (element + WinnerNeurone)->nombre;
}

//calcule de la moyenne
void mean(Data *v, double * resultat ){
  int j,i;
  for(j=0; j<v[0].DataSize; j++){
    double res=0.0;
    for(i=0; i<v[0].taille; i++){
      res += v[i].Data[j];
    }
    resultat[j] = res/v[0].taille;
  }
}

//calcule de la norme
void norme(Data *data){
  int i,j,k;
  for(i=0; i<data[0].taille; i++){
    double s=0;
    for(j=0; j<data[0].DataSize; j++){
      s += pow(data[i].Data[j],2);
    }
    for(k=0; k<data[0].DataSize; k++){
      data[i].Data[k] /= sqrt( s );
    }
  }
}

void readParams(Parametre * params, char *nomFichier){
  FILE *file = fopen(nomFichier,"r");
  size_t row;
  size_t len = 0;
  char * line = NULL;
  char * data, *end;
  int iterator=0;
  while((row = getline(&line, &len, file)) != -1){
    data = strtok(line, " ");
    while(data != NULL){
      if(iterator==0){params->nbRows = strtol(data, &end,10);}
      else if(iterator==1){params->nbColumns = strtol(data, &end,10);}
      else if(iterator==2){params->ecartMin = strtod(data, &end);}
      else if(iterator==3){params->ecartMax = strtod(data, &end);}
      else if(iterator==4){params->alpha = strtod(data, &end);}
      data = strtok(NULL," ");
      iterator++;
    }
  }
  fclose(file);
  return;
}

//lit les données d'un fichier .txt
Data * readData(Data *data, char *nomFichier){
  FILE *file = fopen(nomFichier,"r");
  size_t row;
  size_t len = 0;
  char * line = NULL;
  int DataSize=0, nbLine=0,i,j;

  while((row = getline(&line, &len, file)) != -1){
    nbLine++;//calculate data size

      for(i=0; i<(int)row; i++){
        if(line[i] == ','){DataSize++;}
      }
  }

  DataSize = 4;
  data = malloc(nbLine * sizeof(struct Data));
  for(j=0; j<nbLine; j++){
    data[j].Data = malloc(DataSize * sizeof(double));
  }
  data[0].taille = nbLine;
  data[0].DataSize = DataSize;

  //remplit les données dans le vecteur
  fseek(file, 0, SEEK_SET);
  char * DataLu, *endptr;
  int dat=0, it=0;
  while((row = getline(&line, &len, file)) != -1){
    it=0;
    DataLu = strtok(line, ",");
    while(DataLu != NULL){
      data[dat].Data[it] = strtod(DataLu, &endptr);
      if(it==4){ data[dat].nom = DataLu;}
      DataLu = strtok(NULL,",");
      it++;
    }
    dat++;
  }
  fclose(file);
  return data;
}

//remplit le vecteur de neurone
 void fillVector(Neurone *neurone, double *meanArr, Data *data, Parametre parametre){
   int i,j;
   for(i=0; i<neurone[0].taille; i++){
     neurone[i].vect = malloc(data[0].DataSize * sizeof(double));
      neurone[i].type=0;
      // initiate number of activation for each type
      neurone[i].setoza = 0;
      neurone[i].verginica = 0;
      neurone[i].vergicono = 0;
   }

   
   for(i=0; i<neurone[0].taille; i++){
     for(j=0; j<data[0].DataSize; j++){
       double k = Randomdouble(parametre.ecartMin,parametre.ecartMax);
       neurone[i].vect[j] = meanArr[j] + k;
     }
   }
 }


 void swap(int * indices,int first, int second){ // XOR operation

    int temp = indices[first];
    indices[first]=indices[second];
    indices[second]=temp;
}


// to mix the array of data
 void shuffle(int * indices, int nb){
   int i,rand;
  for(i=0; i<nb ;i++){
     int rand = Randomdouble(0,nb);
     swap(indices,i,rand);
   }
 }


//retrouve les coordonées d'un index
 void trouverCoorData(int nb, int nbcol, int * coord){
   coord[1] = nb%nbcol;
   coord[0] = nb/nbcol;
   return;
 }


  int numberOfStage(int nbrNeurone){
   int voisins=8,i=0;

   while(voisins < nbrNeurone){
    voisins *= 2;
    i += 1;
   }
  
   
   return i;
 }

//diffusion
 void diffusion(Neurone *neurone, double * v, int neuroneGagnant, int taille, int etage, Parametre parametre){
    int coord[2], x, y, k, ligne=parametre.nbRows, colonne=parametre.nbColumns;
    trouverCoorData(neuroneGagnant, colonne, coord);
    int x0 = coord[0]-etage<0?0:coord[0]-etage;
    int y0 = coord[1]-etage<0?0:coord[1]-etage;
    int x1 = coord[0]+etage>ligne-1?ligne-1:coord[0]+etage;
    int y1 = coord[1]+etage>colonne-1?colonne-1:coord[1]+etage;
    for( x = x0; x<=x1; x++){
      for( y = y0; y<=y1; y++){
        //on applique sur chaque neurone la formule
        for(k=0; k<taille; k++){
          neurone[x*colonne + y].vect[k] = neurone[x*colonne + y].vect[k] + parametre.alpha * (v[k] - neurone[x*colonne + y].vect[k]);
        }
      }
    }
   return;
 }


 
double euclidenDistance(Data first, Neurone second, int numberOfVectorValues){ // to calculate the distance between data and neurons
    int t;
    long double calcule =0.0;

    // calculate the eucliden distance
    for(t=0;t<numberOfVectorValues;t++){
       // printf("i == %d  j == %d \n",ii,j );
         //printf("first  %Lf  second  %Lf \n", first.values[i], (second.values[i]));
    calcule += pow((first.Data[t]) - (second.vect[t]),2);
}
    
    return sqrt(calcule);
}


/*
char * toLowerTrim( char * src) {
    char result;
    result = src.ToLower();
    return result;
}*/


char *trim(char *s) {
    char *ptr;
    if (!s)
        return NULL;   // handle NULL string
    if (!*s)
        return s;      // handle empty string
    for (ptr = s + strlen(s) - 1; (ptr >= s) && isspace(*ptr); --ptr);
    //ptr[1] = '\0';
    return s;
}

double euclidenDistance2(Neurone first, Neurone second, int numberOfVectorValues){ // to calculate the distance between data and neurons
    int t;
    long double calcule =0.0;

    // calculate the eucliden distance
    for(t=0;t<numberOfVectorValues;t++){
       // printf("i == %d  j == %d \n",ii,j );
         //printf("first  %Lf  second  %Lf \n", first.values[i], (second.values[i]));
    calcule += pow((first.vect[t]) - (second.vect[t]),2);
}
    
    return sqrt(calcule);
}


void learning(int *indices, Data *data, Neurone *neurone, Parametre params,int nbtour){
int m,i,j,k,etage;
   etage = numberOfStage(neurone[0].taille);
   double alpha=params.alpha;
   int sizeList;


   for(i=0; i<data[0].taille; i++){
     indices[i] = i;
   }

   Liste * winnerList = (Liste*) malloc ( sizeof(winnerList));

   winnerList = (Liste*) realloc (winnerList,2 * sizeof(winnerList));

   for(m=0; m<nbtour; m++){

    // here is the problem
    shuffle(indices,data[0].taille);

    sizeList = 1;
     
      for(i=0; i<data[0].taille ;i++){

        if(i == etage/2 || (i == etage/3))
          etage--;

         winnerList = NULL;
         double bestDist = INT_MAX;

         for(j=0; j<params.nbColumns*params.nbRows; j++){


           double dist=0;

           dist = euclidenDistance(data[indices[i]], neurone[j], 4);
           if(dist<bestDist){
            sizeList = 1;
             bestDist=dist;
             winnerList = NULL;
             
             winnerList=insert(winnerList,j);
           }
           else if(dist == bestDist){
             winnerList=insert(winnerList,j);
             sizeList++;
           }
           
         }

         int choosed = IndexListeChaine(winnerList,sizeList);

          if(nbtour == 1500){
            params.alpha /= 10;
          }
            

           diffusion(neurone, data[indices[i]].Data, choosed, data[0].DataSize, etage, params);

           char* choosedype = data[choosed].nom;


        // counting nulbers of time where a neurone was activated then label it 
           //depending on the number of activations

        if(indices[i] < 50 ){ // trim(choosedype) == trim(setoza)
          neurone[choosed].setoza++;
        }

        else if (indices[i] < 100){
          neurone[choosed].verginica++;
        }

        else if (indices[i] < 150){
          neurone[choosed].vergicono++;
        }



        /*
        switch(test)
          {
          case '0':
             neurone[choosed].setoza++;
             //printf("setoza\n");
              break;

          case '1':
              neurone[choosed].verginica++;
              break;

          case '*':
              neurone[choosed].vergicono++;
              break;

          }*/
      
      }
    }

  /*double oldDistance, newDistance;

  Neurone aux;
  
  for(i=0; i<59; i++){
    printf("neurone i  =  %f \n",neurone[i].vect[0] );

    oldDistance = euclidenDistance2(neurone[i], neurone[0], 4);

    for(j=i; j<60; j++){
      newDistance = euclidenDistance2(neurone[i], neurone[j], 4);

      if(newDistance > oldDistance ){
        aux = neurone[j];
        neurone[j] = neurone[i];
        neurone[i] = aux;
      }

    }
  }*/

}



void labeling(Neurone * neurone, Data * data ){

int label, i,k,j, choosedype,type;

  for(j=0; j<60; j++){

    choosedype = neurone[j].setoza;
    type = 1;
  
    if(choosedype < neurone[j].verginica){
      choosedype = neurone[j].verginica;
      type = 2;
    }

    if(choosedype < neurone[j].vergicono){
      choosedype = neurone[j].vergicono;
      type = 3;
    }


    neurone[j].type = type;

  }
}




//affiche les neurones
void showNeurons(Neurone * neurone, Parametre parametre){
  printf("------------- Self Organizing Map (SOM) \n");
  int i, j, ligne=parametre.nbRows, colonne=parametre.nbColumns;
  for(i=0; i<ligne; i++){
    for(j=0; j<colonne; j++){

      switch(neurone[i*colonne+j].type)
          {
          case 1:
             printf("\033[45m%d\033[0m",neurone[i*colonne+j].type);
              break;

          case 2:
              printf("\033[46m%d\033[0m",neurone[i*colonne+j].type);
              break;

          case 3:
              printf("\033[43m%d\033[0m",neurone[i*colonne+j].type);
              break;

            }
    }
    printf("\n");
  }
}


//remplit le vecteur de neurone
 void fillVector_kmeans(Neurone *clusters, double *meanArr, Parametre parametre, int nbClusters){

   int i,j;

  clusters[0].nbClusters = nbClusters;
   for(i=0; i<3; i++){
     clusters[i].vect = malloc(4 * sizeof(double));
      clusters[i].type=i+1;
      // initiate number of activation for each type

     // printf("clusters[i].type %d \n", clusters[i].type);
      

   }


   
   for(i=0; i<clusters[0].nbClusters; i++){
     for(j=0; j<4; j++){
       double k = Randomdouble(parametre.ecartMin,parametre.ecartMax);
       clusters[i].vect[j] = meanArr[j] + k;
       //printf("clusters[i] %f ", clusters[i].vect[j]);
     }
     printf("\n");
   }
 }


//calcule de la moyenne
void meanByType(Data *data, Neurone* clusters ){
  int j,i,k;
  double* countType = (double*)malloc(clusters[0].nbClusters * sizeof(double));

  Data* meanArr = (Data*)malloc(clusters[0].nbClusters * sizeof(Data));

  // initiating variables

  for(i=0; i<clusters[0].nbClusters; i++){

    meanArr[i].Data = (double*)malloc(data[0].DataSize * sizeof(double));
    countType[i] = 0;
    for(k=0; k<data[0].DataSize; k++){
      meanArr[i].Data[k] = 0;
    }
  }


  for(i=0; i<150; i++){

    for(j=0; j<clusters[0].nbClusters; j++){

      if(clusters[j].type == data[i].type){
        countType[j]++;
        //printf("countType %d %d \n",j,countType[j] );

        for(k=0; k<data[0].DataSize; k++){  ////////////// v[0].DataSize

            meanArr[j].Data[k] += data[i].Data[k];
            //printf("-------------meanArr[j].Data[%d] = %f \n",j, meanArr[j].Data[k]);

          //resultat[j] = res/v[0].taille;
        }
      }
    }
  }


  // calculate the mean

  for(j=0; j<clusters[0].nbClusters; j++){

    for(k=0; k<data[0].DataSize; k++){

      meanArr[j].Data[k] = meanArr[j].Data[k] /countType[j];

      clusters[j].vect[k] = meanArr[j].Data[k];
    }
  }
}


void typeAffectation(Neurone * clusters, Data *data , int* indices){
  int i,k,j,type;
  double oldDistance, newDistance;
  for(i=0; i<data[0].taille; i++){

    oldDistance = euclidenDistance(data[indices[i]], clusters[0], data[0].DataSize);
    type = clusters[0].type;


    for(j=0; j<clusters[0].nbClusters; j++){
      newDistance = euclidenDistance(data[indices[i]], clusters[j], data[0].DataSize);
      if(newDistance < oldDistance){
        type = clusters[j].type;
        oldDistance = newDistance;
      }
    }

      // type affectation
      data[indices[i]].type = type;

  }
}

void Kmeans(Neurone * clusters, Data *data , Parametre params){
  int i,k,l,j, type;
  double oldDistance, newDistance;
  int *indices = malloc(data[0].taille * sizeof(int));
     for(i=0; i<data[0].taille; i++){
     indices[i] = i;
   }

  // number of totale iteration for kmeans
  for(k=0; k<300; k++){

    shuffle(indices,data[0].taille);

    typeAffectation(clusters,data,indices);

    // update clusters
    meanByType(data, clusters );

  }

  // sort kmeans types to print them

  int aux;
  
  for(i=0; i<data[0].taille-1; i++){

    for(j=i; j<data[0].taille; j++){

      if(data[i].type > data[j].type ){
        aux = data[i].type;
        data[i].type = data[j].type;
        data[j].type = aux;
      }

    }
  }

  printf("---------------Kmeans-----------------------\n");

  for(i=0; i<data[0].taille; i++){

    switch(data[i].type)
      {
      case 1:
             printf("\033[45m%d\033[0m",data[i].type);
              break;

      case 2:
             printf("\033[46m%d\033[0m",data[i].type);
             break;

      case 3:
             printf("\033[43m%d\033[0m",data[i].type);
             break;

      }

    if(i%10 == 0)
      printf("\n");
  }
  printf("\n");

}



int main(int argc, char * argv[]){
  if (argc<3) {
    printf("you must give all arguments\n");
    exit(0);
  }

  srand(time(NULL));

  Parametre params;
  readParams(&params, argv[1]);
  Data *data = NULL ;
  data = readData(data, argv[2]);

  int *randomIndexesArray = malloc(data[0].taille * sizeof(int));
  int neuroneSize = params.nbRows * params.nbColumns;
  double *moyenneArr = malloc(data[0].DataSize * sizeof(double));
  Neurone * neurone = malloc(neuroneSize * sizeof(struct Neurone));
  neurone[0].taille = neuroneSize;

  // Self Organizing Map part

  // normalyse data
  norme(data);

  // calculate the mean of all data to center it later
  mean(data, moyenneArr);

  // fill the map
  fillVector(neurone, moyenneArr, data, params);

  // two steps of learning : first with big rate(0,7) the second with low rate (0,07)

  // learning step 1
  learning(randomIndexesArray, data, neurone, params,500);
  // learning step 2
  learning(randomIndexesArray,data, neurone, params,1500);

  // label the data
  labeling(neurone,data );

  // print label
  showNeurons(neurone, params);

  // Kmeans part

  Neurone * clusters = malloc(3 * sizeof(struct Neurone));

  // fill the clusters

  fillVector_kmeans(clusters, moyenneArr, params,3);

  // apply Kmeans

  Kmeans(clusters,data ,params);




  free(neurone);
  free(data);

  return 0;
}
