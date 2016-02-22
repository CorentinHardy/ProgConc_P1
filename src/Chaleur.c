#include "Chaleur.h"
#include <math.h>
#include <stdio.h>
#include <getopt.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
  
  //Pour le calcul de la taille du probleme, de base 4
  int puissance = 4;

  //Calcul du temps moyen d execution, par defaut non
  int calculTps = 1;

  //Nombre d iteration
  int numIter = 100;

  //Choix d afficher seulement la premiere et la derniere iteration
  int flagA = 0;

  //Parametre de base pour differentes options
  char* sparam = "024";

  char* eparam = "012345";

  char* tparam = "13";

  //Applications des parametres passe a la fonction
  int c;
  extern char * optarg; 
  extern int optind, opterr; 
  while ((c = getopt(argc, argv, "s:ami:e:t:")) != -1)
    switch (c) {
      case 's':
        sparam = optarg;
        break;
      case 'a':
        flagA = 1;
        break;
      case 'm':
        calculTps = 1;
        break;
      case 'i':
        numIter = atoi(optarg);
        break;
      case 'e':
        eparam = optarg;
        break;
      case 't':
        tparam = optarg;
        break;
      case '?':
        if (optopt == 's'){
          fprintf (stderr, "Option -%c requiere un argument.\n", optopt);
          fprintf(stderr, "taille de la matric\n");
        }else if (optopt == 'i'){
          fprintf (stderr, "Option -%c requiere un argument.\n", optopt);
          fprintf(stderr, "nombre d iteration\n");
        }else if (optopt == 'e'){
          fprintf (stderr, "Option -%c requiere un argument.\n", optopt);
          fprintf(stderr, "etape a executer\n");
        }else if (optopt == 't'){
          fprintf (stderr, "Option -%c requiere un argument.\n", optopt);
          fprintf(stderr, "nombre de thread a creer\n");
        }else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else{
          fprintf (stderr,
                   "option inconnu `\\x%x'.\n",
                   optopt);
          return 1;
        }
        break;
      }
  
  // On prend toutes les etapes a executer
  for(int i = 0; i < strlen(eparam); i++){
    switch(eparam[i]){
      case '0':
        //Pour chaque etape on prend tout les tailles de grille a utiliser
        for(int j = 0; j < strlen(sparam); j++){
          //Pour chaque taille de grille on prend les differents nombre de thread a utiliser
          for(int k = 0; k < strlen(tparam); k++) {
            //Si on a demande la moyenne du temps d execution on la calcule
            if(calculTps)
              calculMoyTps(numIter,puissance+sparam[j] - '0',tparam[k] - '0', flagA);
            else 
              simulation(numIter,puissance+sparam[j] - '0',tparam[k] - '0', flagA);  
          }
        }
        break;
    }
  }      
}

//Calcule la moyenne du temps d execution ce la simulation 
static void calculMoyTps(int numIter, int puissance, int thread, int flagA) {
  int NUM_ITER = 10;
  clock_t tps_clock[NUM_ITER];
  time_t tps_time[NUM_ITER];

  //On effectue les NUM_ITER simulations et on stocke les temps d'execution
  for(int i = 0; i < NUM_ITER; i++){
    time_t t1 = time(NULL);
    clock_t c1 = clock();

    simulation(numIter,puissance,thread,flagA);

    clock_t c2 = clock();
    time_t t2 = time(NULL);

    tps_clock[i] = c2 - c1;
    tps_time[i] = difftime(t2, t1);
  }

  //On cherche les deux extremes et on les elimines
  int max_clock = 0;
  int min_clock = 0;

  int max_time = 0;
  int min_time = 0;

  for(int i = 1; i < NUM_ITER; i++){

    if(tps_clock[i] > tps_clock[max_clock])
      max_clock = i;
    if(tps_clock[i] < tps_clock[min_clock])
      min_clock = i;

    if(tps_time[i] > tps_time[max_time])
      max_time = i;
    if(tps_time[i] < tps_time[min_time])
      min_time = i;
  }
  tps_clock[max_clock] = 0;
  tps_clock[min_clock] = 0;

  tps_time[max_time] = 0;
  tps_time[min_time] = 0;


  //On calcule la moyenne en ramenant le temps en secondes
  double moyenne_clock;
  double moyenne_time;

  for(int i = 0; i < NUM_ITER; i++) {
    moyenne_clock += (double)tps_clock[i]/(double)CLOCKS_PER_SEC;
    moyenne_time += (double)tps_time[i];
  }

  moyenne_clock = moyenne_clock / (0.0 + NUM_ITER-2);
  moyenne_time = moyenne_time / (0.0 + NUM_ITER-2);

  printf("Moyenne d'execution du CPU: %g \n", moyenne_clock);
  printf("Temps d'execution moyen: %g \n", moyenne_time);

}

static void simulation(int numIter, int puissance, int thread, int flagA) {
  //Initialisation des variables
  int tab_Size = pow(2,puissance);
 
  float grille[tab_Size+2][tab_Size+2];
  float grilleResult[tab_Size+2][tab_Size+2];
      
  float* g[tab_Size+2]; 
  float* gRes[tab_Size+2];

  for(int i = 0; i<tab_Size+2; i++){
    g[i] = &grille[i];
    gRes[i] = &grilleResult[i];
  }

  //On initialise les grilles
  initialiser(g,tab_Size);
  initialiser(gRes,tab_Size);

  //On remplit la grille avec les temperatures fixe
  appliquerTempFixe(g,tab_Size,puissance);

  //Application de l algo K fois
  for(int k = 0; k < numIter; k++) {

  //On parcourt le tableau reel (en enlevant les COEF_BORDs qui sont a temperature fixe)
    for(int i = 1; i < tab_Size+1; i++){
      for(int j = 1; j < tab_Size+1; j++){
        float value = grille[i][j];
        if(value != 0)
          transfert(gRes,i,j,value);
      }
    }

    //On recopie le tableau de resultat dans le tableau normal (sans les bords vu que l on va de toute façon les reinitialiser)
    for(int i = 1; i<tab_Size+1; i++)
      for(int j = 1; j<tab_Size+1; j++) 
        grille[i][j] = grilleResult[i][j];

    //On reinitialise la grille de resultat
    initialiser(gRes,tab_Size);

    //On parcourt la grille en remettant les bonnes valeurs pour les zones a valeur fixe
    appliquerTempFixe(g,tab_Size,puissance);

    //On affiche la grille
    if(flagA) {
      if(k == 0 || k == numIter-1)
        afficher(g, tab_Size+2, k+1, puissance);
    }
    else
      afficher(g, tab_Size+2, k+1, puissance);
  }
}


//Met toute les valeurs de la grille a 0
static void initialiser(float** grille, int size){
  for(int i = 0; i < size+2; i++)
    for(int j = 0; j < size+2; j++) 
      grille[i][j] = 0;
}

// Applique les temperatures fixes
static void appliquerTempFixe(float** grille, int size, int puissance) {
  //On remplit l exterieur de la grille avec la temperature la plus basse
  for(int j = 0; j < size+2; j++) {
    grille[0][j] = TEMP_FROID; 
    grille[size+1][j] = TEMP_FROID;   
  } 

  for(int i = 0; i < size+2; i++) {
    grille[i][0] = TEMP_FROID;
    grille[i][size+1] = TEMP_FROID;
  }

  //On remplit le coeur avec la temperature la plus haute
  for(int i = pow(2,puissance-1)-pow(2,puissance-4) ; i < pow(2,puissance-1)+pow(2,puissance-4) ; i++)
    for(int j = pow(2,puissance-1)-pow(2,puissance-4) ; j < pow(2,puissance-1)+pow(2,puissance-4) ; j++) 
      grille[i][j] = TEMP_CHAUD;
}

// Affiche la grille
static void afficher(float** grille, int size, int nbExec, int puissance) {
  printf("\n\n<------------------- Execution n° %i ------------------->\n\n", nbExec);
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      printf("%i ",(int)grille[i][j]);
    }
    printf("\n");
  }
}

//Ajoute dans la grille de resultat la chaleur transfere a chaque case adjacente
static void transfert(float** grilleResult, int x, int y, float value) {
  grilleResult[x-1][y-1] += value * COEF_ANGLE;
  grilleResult[x-1][y+1] += value * COEF_ANGLE;
  grilleResult[x+1][y-1] += value * COEF_ANGLE;
  grilleResult[x+1][y+1] += value * COEF_ANGLE;
  grilleResult[x+1][y] += value * COEF_BORD;
  grilleResult[x-1][y] += value * COEF_BORD;
  grilleResult[x][y+1] += value * COEF_BORD;
  grilleResult[x][y-1] += value * COEF_BORD;
  grilleResult[x][y] += value * COEF_CENTRE;
}
/*
static float taylor(int x, int y){
  return 0.0;
}
/*
 * on calcule en considerant une matrice dont on a les 9 valeurs proches,
 * on connait aussi les coefficients a appliquer, avec COEF_ANGLE=val11=val13=val31=val33
 * et COEF_BORD=val12=val21=val23=val32.
 * /
static float cellule_9(float** tab_9, int x, int y){
  return COEF_CENTRE * tab_9[x][y] + COEF_BORD * (tab_9[x+1][y] + tab_9[x][y+1] 
    + tab_9[x-1][y] + tab_9[x][y-1]) + COEF_ANGLE * (tab_9[x+1][y+1] 
    + tab_9[x-1][y+1] + tab_9[x-1][y-1] + tab_9[x+1][y-1]);
}
//*/
