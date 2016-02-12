#include "Chaleur.h"

int main(int argc, char *argv[]) {
  opterr = 0;
  while ((c = getopt (argc, argv, "th:")) != -1)
    switch (c)
      {
      case 't':
        // on change la taille du tableau
        break;
      case 'h':
        // on affiche des trucs pour aider
        break;
      case '?':
        if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
      }
}

static float taylor(int x, int y){
  return 0.0;
}

static float val22 = 4.5;
static float val12 = 1/9;
static float val11 = 1/36;
/* 
 * on calcule en considérant une matrice dont on a les 9 valeurs proches,
 * on connait aussi les coefficients à appliquer, avec val11=val13=val31=val33
 * et val12=val21=val23=val32.
 */
static float cellule_9(int** tab_9, int x, int y){
  return val22 * tab_9[x][y] + val12 * (tab_9[x+1][y] + tab_9[x][y+1] 
    + tab_9[x-1][y] + tab_9[x][y-1]) + val11 * (tab_9[x+1][y+1] 
    + tab_9[x-1][y+1] + tab_9[x-1][y-1] + tab_9[x+1][y-1]);
}

//int*