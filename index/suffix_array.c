#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TITLE  "Suffix array construction algorithm\n"
#define AUTHOR "Jan Holub, 2026\n"
#define HRULE  "-----------------------------------\n"
#define USAGE  "Usage: suffix_array <TEXT>\n"

#define MAX_TEXT_LENGTH sizeof(int)

typedef struct SA {
  char *t;  // text
  int n, *suffix_array;  // length, suffix array
} SA_type;


/* --------------------------------------------------------------------------*/
                                  // suffcmp - compares cyclic rotations
int suffcmp(char* str, int n, int i, int j) {
  int res, l;

  l=n;
  while (l>=0 && (str[i%n]==str[j%n])) {
    i++; j++; l--;
  }
  res=str[i%n]-str[j%n];
  return res;
}

/* --------------------------------------------------------------------------*/
SA_type* SA_init(char* str, int n) {
  int i, b, c, *sa;
  SA_type* SA;

  SA = (SA_type *)malloc(sizeof(SA_type));
  if(SA==NULL) {
    fprintf(stderr, "Not enough memory!\nExiting.\n");
    exit (-1);
  }
  SA->t = (char *)malloc(n+1);
  if (SA->t==NULL) {
    fprintf(stderr, "Not enough memory!\nExiting.\n");
    exit (-1);
  }
  SA->n = n;
  strcpy(SA->t,str);
  SA->suffix_array = (int*) malloc ((n+1) * sizeof(int));
  if (SA->suffix_array==NULL) {
    fprintf(stderr, "Not enough memory!\nExiting.\n");
    exit (-1);
  }
  return SA;
}

/* --------------------------------------------------------------------------*/
SA_type* suffix_array_naive(char* str, int n) {
  int i, b, c, *sa;
  SA_type* SA;

  SA=SA_init(str, n);

  sa = SA->suffix_array;
  for (i = 0; i < n; i++)
    sa[i]=i;
  do {
    b=0;                   // b=1 indicates a change in the last cycle
    for (i = 1; i < n; i++)
      //      if (strcmp(str+sa[i-1], str+sa[i]) > 0){    // cyclic T sorted
      if (suffcmp(str, n, sa[i-1], sa[i]) > 0){   // T$ sorted
	b=1;              // mark the change
	c=sa[i];         // exchange sa[i] <-> sa[i-1]
	sa[i]=sa[i-1];
	sa[i-1]=c;
      };
  } while (b==1);
  
  for (i = 0; i < n; i++){  // print sorted suffixes
    printf(" %2d %2d  %s\n",i,sa[i],str+sa[i]);
  }
    
  return SA;
}

/* --------------------------------------------------------------------------*/
void print_final_SA(SA_type* SA) {
  int i,n;
  
  n=SA->n;

  printf("\nIndex:  ");
  for (i = 0; i <= n; i++)
    printf(" %2d", i);

  printf("\nString:  ");
  for (i = 0; i < n; i++)
    printf("%2c ", SA->t[i]);
  printf("%2c ", '$');

  printf("\n SA[i]:  ");
  for (i = 0; i < n; i++)
    printf("%2d ", SA->suffix_array[i]);
  printf("\n");
}
  
// function main -------------------------------------------------------------
int main(int argc, char **argv) {
  int *sa, *C;
  char *L;
  int m, n, i;
  SA_type* SA;

  printf(HRULE);
  printf(TITLE);
  printf(AUTHOR);
  printf(HRULE);
  if (argc < 2) {
    printf(USAGE);
    return EXIT_FAILURE;
  }

  printf("T=%s\n", argv[1]);
  n = strlen(argv[1]);
  printf("n=%d\n", n);

  printf("Preprocessing:\n");
  SA = suffix_array_naive(argv[1], n);
  print_final_SA(SA);
  return EXIT_SUCCESS;
}
