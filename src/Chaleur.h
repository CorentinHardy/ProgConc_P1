/**

@author Corentin Hardy, Edgar Persenda
Date : 22/02/2015

**/

#ifndef __CHALEUR__
#define __CHALEUR__

int TEMP_CHAUD = 666;
int TEMP_FROID = 5;

float COEF_ANGLE = 1/36.0;
float COEF_BORD = 4/36.0;
float COEF_CENTRE = 16/36.0;

static void simulation(int numIter, int puissance, int thread, int flagA, int affichage);

static void afficher(float** grille, int size, int nbExec);

static void transfert(float** grille, float** grilleResult, int x, int y);

static void initialiser(float** grille, int size, int valeur);

static void appliquerTempFixe(float** grille, int size, int puissance);

static void calculMoyTps(int numIter, int puissance, int thread, int flagA);
#endif