#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TITLE  "Border Array construction algorithm\n"
#define AUTHOR "Jan Holub, 2023\n"
#define HRULE  "-----------------------------------\n"
#define USAGE  "Usage: ba <TEXT>\n"



/* --------------------------------------------------------------------------*/
int* border_array(char* str, int m) {
  int i, b, *ba;

  ba = (int*) malloc ((m+1) * sizeof(int));
  ba[0] = 0;
  for (i = 1; i < m; i++) {
    b = ba[i-1];
    while (b > 0 && str[i] != str[b]) 
      b = ba[b-1];
    ba[i] = ((str[i] == str[b]) ? b + 1 : 0);
  }
  return ba;
}

/* --------------------------------------------------------------------------*/
void print_table(int* ba, char* str, int m) {
  int i;
  
  printf("\nIndex:  ");
  for (i = 0; i < m; i++)
    printf(" %2d", i);

  printf("\nString:  ");
  for (i = 0; i < m; i++)
    printf("%2c ", str[i]);

  printf("\nBorder:  ");
  for (i = 0; i < m; i++)
    printf("%2d ", ba[i]);
  printf("\n");  
}

// function main -------------------------------------------------------------
int main (int argc, char *argv[]) {
  int *ba;
  int n;

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
  ba = border_array(argv[1], n);
  print_table(ba, argv[1], n);
  free(ba);  
  return EXIT_SUCCESS;
}
