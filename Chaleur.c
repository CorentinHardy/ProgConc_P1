#include "Chaleur.h"

int main(int argc, char *argv[]) {
  opterr = 0;
  while ((c = getopt (argc, argv, "abc:")) != -1)
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

int* 