#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TITLE  "Naive algorithm\n"
#define AUTHOR "Jan Holub, 2023\n"
#define HRULE  "---------------\n"
#define USAGE  "Usage: naive <TEXT> <PATTERN>\n"

char fillers[1024];

/* --------------------------------------------------------------------------*/
void naive(char* text, int n, char* pattern, int m) {
  int i, j;

  for (i = 0; i < n ;i++)
    printf("%d", i%10);
  printf("\n");
  printf("%s\n", text);
  for (i = 0; i <= n-m; i++) {
    if (i>0) printf("%*c", i, ' ');
    j=0;
    while (j<m && text[i+j] == pattern[j]){ 
      printf("%c", pattern[j]);
      if (++j == m) 
        printf("%*c  match on position %d", n-i, ' ', i);
    }
    if (j<m) printf("%c", pattern[j]);
    printf("\n");
    }
}

/* --------------------------------------------------------------------------*/
void Naive(char* text, int n, char* pattern, int m) {
  int i, j;

  for (i = 0; i <= n-m; i++) {
    j=0;
    while (j<m && text[i+j] == pattern[j]){ 
      if (++j == m) 
        printf("  match on position %d\n", i);
    }
  }
}


// function main -------------------------------------------------------------
int main(int argc, char **argv) {
  int i, n, m;

  printf(HRULE);
  printf(TITLE);
  printf(AUTHOR);
  printf(HRULE);
  if (argc < 3) {
    printf(USAGE);
    return EXIT_FAILURE;
  }
  memset(fillers, (int)'?', 1024);

  printf("T=%s\n", argv[1]);
  printf("P=%s\n", argv[2]);
  n = strlen(argv[1]);
  m = strlen(argv[2]);
  naive(argv[1], n, argv[2], m);
  //  Naive(argv[1], n, argv[2], m);
  return EXIT_SUCCESS;
}
