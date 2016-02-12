#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char **argv)
{
  int c;
  int matrix_size = 4;
  int nb_iterat = 10000;

  int index;

  opterr = 0;
  while ((c = getopt (argc, argv, "hsimMaet:")) != -1)
    switch (c)
      {
      case 'h':
        fprintf(stderr, "s, i, m, M, a, e, t\n");
        return 0;
      case 's':
        matrix_size = (int) optarg;
        break;
      case 'i':
        nb_iterat = (int) optarg;
        break;
      case 'm':
      case 'M':
      case 'a':
      case 'e':
      case 't':
        fprintf(stderr, "-%c is not implemeted yet\n", optopt);  
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
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
      }
  printf ("matrix_size = %d, nb_iterat = %d \n",
          matrix_size, nb_iterat);

  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);
  return 0;
}