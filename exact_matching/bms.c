#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 256

#define TITLE  "Boyer-Moore-Sunday algorithm\n"
#define AUTHOR "Jan Holub, 2023\n"
#define HRULE  "-------------------------------\n"
#define USAGE  "Usage: bms <TEXT> <PATTERN>\n"

char fillers[1024];
char spaces[1024];

/* --------------------------------------------------------------------------*/
int* compute_BCS(char* pattern, int m) {
  int i;
  int *BCS = (int*) malloc (ALPHABET_SIZE * sizeof(int));

  for (i = 0; i < ALPHABET_SIZE; i++)
    BCS[i] = m+1;
  for (i = 0; i < m; i++)
    BCS[pattern[i]] = m - i;

  return BCS;
}

/* --------------------------------------------------------------------------*/
void print_BCS(int* bcs, int m) {
  int i;
  
  printf("\nIndex:  ");
  for (i = 0; i < ALPHABET_SIZE; i++)
    if (bcs[i] != m+1)
      printf(" %3c", i);

  printf(" ???");

  printf("\nBCS:     ");
  for (i = 0; i < ALPHABET_SIZE; i++)
    if (bcs[i] != m+1)
      printf("%3d ", bcs[i]);
  
  printf("%3d ", m+1);

  printf("\n");  
}

/* --------------------------------------------------------------------------*/
void boyer_moore_sunday(char* text, int n, char* pattern, int m) {
  int i, j, *BCS;
  
  printf("Preprocessing:\n");
  BCS = compute_BCS(pattern, m);
  print_BCS(BCS, m);

  printf("\nProcessing:\n");
  for (i = 0; i < n ;i++)
    printf("%d", i%10);
  printf("\n");
  printf("%s\n", text);
  for (i = m - 1; i < n; i += BCS[text[i+1]]) {
    j = 0;
    while (j<m && text[i-j] == pattern[m-1-j])
      j++;
    if (j == m) {
      printf("%*.s%.*s%.*s", i-m+1, spaces, m-j, fillers, j, pattern+m-j);
      printf("%*c  match on position %2d\n", n-i, ' ', i-j+1);
    } else {
      printf("%*.s%.*s%.*s", i-m+1, spaces, m-j-1, fillers, j+1, pattern+m-j-1);
      printf("\n");
    }
  }
  free(BCS);
}

/* --------------------------------------------------------------------------*/
void Boyer_Moore_Sunday(char* text, int n, char* pattern, int m) {
  int i, j, *BCS;
  
  BCS = compute_BCS(pattern, m);
  for (i = m - 1; i < n; i += BCS[text[i+1]]) {
    j = 0;
    while (j<m && text[i-j] == pattern[m-1-j])
      j++;
    if (j == m) {
      printf("  match on position %2d\n", i-j+1);
    }
  }
  free(BCS);
}

// function main -------------------------------------------------------------
int main(int argc, char **argv) {
  int n, m;

  printf(HRULE);
  printf(TITLE);
  printf(AUTHOR);
  printf(HRULE);
  if (argc < 3) {
    printf(USAGE);
    return EXIT_FAILURE;
  }
  memset(fillers, (int)'.', 1024);
  memset(spaces, (int)' ', 1024);

  printf("T=%s\n", argv[1]);
  printf("P=%s\n", argv[2]);
  n = strlen(argv[1]);
  m = strlen(argv[2]);
  boyer_moore_sunday(argv[1], n, argv[2], m);
  //  Boyer_Moore_Sunday(argv[1], n, argv[2], m);

  return EXIT_SUCCESS;
}
