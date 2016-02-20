#include "Chaleur.h"
#include <math.h>
#include <stdio.h>
#include <getopt.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
  
  //Pour le calcul de la taille du probleme, de base 4
  int puissance = 4;

  //Calcul du temps moyen d'execution, par defaut non
  int calculTps = 0;

  //Nombre d'iteration
  int numIter = 1000;

  //Parametre de base pour differentes options
  char* sparam = "024";

  char* eparam = "012345";

  char* tparam = "13";

  //Applications des paramètres passé a la fonction
  int c;
  extern char * optarg; 
  extern int optind, opterr; 
  while ((c = getopt(argc, argv, "s:mi:e:t:")) != -1) switch (c)
  {
    case 's':
      printf("DETECT S \n");
      sparam = optarg;
      break;
    case 'm':
      printf("DETECT M \n");
      calculTps = 1;
      break;
    case 'i':
      printf("DETECT I \n");
      numIter = atoi(optarg);
      break;
    case 'e':
      printf("DETECT E \n");
      eparam = optarg;
      break;
    case 't':
      printf("DETECT T \n");
      tparam = optarg;
      break;
    case '?':
      if (optopt == 'i'){
        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        fprintf(stderr, "It is the number of the iteration.\n");
      }else if (optopt == 's'){
        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        fprintf(stderr, "It is the size of the matrix.\n");
      }else if (isprint (optopt))
        fprintf (stderr, "Unknown option `-%c'.\n", optopt);
      else{
        fprintf (stderr,
                 "Unknown option character `\\x%x'.\n",
                 optopt);
        return 1;
      }
      break;
  }
  /**
  // On prend toutes les étapes à executer
  for(int i = 0; i < strlen(eparam); i++){
    switch(eparam[i]){
      case '0':
        //Po0ur chaque etape on prend tout les tailles de grille à utiliser
        for(int j = 0; j < strlen(sparam); j++){
          //Pour chaque taille de grille on prend les differents nombre de thread à utiliser
          for(int k = 0; k < strlen(tparam); k++) {
            //Si on a demandé la moyenne du temps d'execution on la calcule
            printf("%i\n", sparam[j]);
            if(calculTps)
              calculMoyTps(numIter,puissance+sparam[j],tparam[k]);
            else 
              simulation(numIter,puissance+sparam[j],tparam[k]);                      
          }
        }
        break;
    }
  }
  **/
  calculMoyTps(numIter,puissance,1);
  //simulation(numIter,puissance,1);
      
}

//Calcule la moyenne du temps d'execution ce la simulatio 
static void calculMoyTps(int numIter, int puissance, int thread) {
  int NUM_ITER = 10;
  clock_t tps[NUM_ITER];
  //On effectue les NUM_ITER simulations et on stocke les temps d'execution
  for(int i = 0; i < NUM_ITER; i++){
    clock_t t1 = clock();
    simulation(numIter,puissance,thread);
    clock_t t2 = clock();
    tps[i] = t2 - t1;
  }

  //On cherche les deux extremes et on les elimines
  int max = 0;
  int min = 0;
    for(int i = 1; i < NUM_ITER; i++){
      if(tps[i] > tps[max])
        max = i;
      if(tps[i] < tps[min])
        min = i;
  }
  tps[max] = 0;
  tps[min] = 0;

  //On calcule la moyenne en ramenant le temps en secondes
  double moyenne;
  for(int i = 0; i < NUM_ITER; i++)
    moyenne += (double)tps[i]/(double)CLOCKS_PER_SEC;
  moyenne = moyenne / (0.0 + NUM_ITER-2);

  printf("Moyenne d'execution: %g \n", moyenne);
}

static void simulation(int numIter, int puissance, int thread) {
      //Initialisation des variables
      int tab_Size = pow(2,puissance);
 
      float grille[tab_Size+2][tab_Size+2];
      float grilleResult[tab_Size+2][tab_Size+2];
                                      printf("TEST\n");
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

      //Application de l'algo K fois
      for(int k = 0; k < numIter; k++) {

      //On parcourt le tableau réel (en enlevant les COEF_BORDs qui sont à temperature fixe)
      for(int i = 1; i < tab_Size+1; i++){
        for(int j = 1; j < tab_Size+1; j++){
          float value = grille[i][j];
          if(value != 0)
            transfert(gRes,i,j,value);
        }
      }

      //On recopie le tableau de resultat dans le tableau normal (sans les bords vu que l'on va de toute façon les reinitialiser)
      for(int i = 1; i<tab_Size+1; i++)
        for(int j = 1; j<tab_Size+1; j++) 
          grille[i][j] = grilleResult[i][j];

      //On reinitialise la grille de resultat
      initialiser(gRes,tab_Size);

      //On parcourt la grille en remettant les bonnes valeurs pour les zones à valeur fixe
      appliquerTempFixe(g,tab_Size,puissance);

      //On affiche la grille
      afficher(g, tab_Size+2, k+1);
  }
}


//Met toute les valeurs de la grille a 0
static void initialiser(float** grille, int size){
      for(int i = 0; i < size+2; i++) {
        for(int j = 0; j < size+2; j++) {
          grille[i][j] = 0;
        }
      }
}

// Applique les temperatures fixes
static void appliquerTempFixe(float** grille, int size, int puissance) {
      //On remplit l'exterieur de la grille avec la temperature la plus basse
      for(int j = 0; j < size+2; j++) {
        grille[0][j] = LOW_HEAT; 
        grille[size+1][j] = LOW_HEAT;   
      } 

      for(int i = 0; i < size+2; i++) {
        grille[i][0] = LOW_HEAT;
        grille[i][size+1] = LOW_HEAT;
      }
      //On remplit le coeur avec la temperature la plus haute
      for(int i = pow(2,puissance-1)-pow(2,puissance-4) ; i < pow(2,puissance-1)+pow(2,puissance-4) ; i++){
          for(int j = pow(2,puissance-1)-pow(2,puissance-4) ; j < pow(2,puissance-1)+pow(2,puissance-4) ; j++) {
            grille[i][j] = HIGH_HEAT;
          }
      } 

}

// Affiche la grille
static void afficher(float** grille, int size, int nbExec) {
        printf("\n\n<------------------- Execution n° %i ------------------->\n\n", nbExec);
        for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
          printf("%4i ",(int)grille[i][j]);
        }
        printf("\n");
      }
}

//Ajoute dans la grille de resultat la chaleur transféré a chaque case adjacente
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

static float COEF_ANGLE = 4.5;
static float COEF_BORD = 1/9;
static float COEF_ANGLE = 1/36;

 * on calcule en considérant une matrice dont on a les 9 valeurs proches,
 * on connait aussi les coefficients à appliquer, avec COEF_ANGLE=val11=val13=val31=val33
 * et COEF_BORD=val12=val21=val23=val32.
 
static float cellule_9(float** tab_9, int x, int y){
  return COEF_CENTRE * tab_9[x][y] + COEF_BORD * (tab_9[x+1][y] + tab_9[x][y+1] 
    + tab_9[x-1][y] + tab_9[x][y-1]) + COEF_ANGLE * (tab_9[x+1][y+1] 
    + tab_9[x-1][y+1] + tab_9[x-1][y-1] + tab_9[x+1][y-1]);
}

*/