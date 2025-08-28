#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TITLE  "Knuth-Morris-Pratt algorithm\n"
#define AUTHOR "Jan Holub, 2023\n"
#define HRULE  "----------------------------\n"
#define USAGE  "Usage: kmp <TEXT> <PATTERN>\n"

char fillers[1024];

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
int* mp_function(char* pattern, int* ba, int m) {
  int i, k;
  int *mp = (int*) malloc ((m+1) * sizeof(int));

  mp[0] = 0;
  for (i = 1; i <= m; i++) {
    mp[i] = ba[i-1];
  }
  return mp;
}

/* --------------------------------------------------------------------------*/
int* kmp_function(char* pattern, int* ba, int m) {
  int i, k;
  int *mp = (int*) malloc ((m+1) * sizeof(int));
  int *kmp = (int*) malloc ((m+1) * sizeof(int));

  mp[0] = 0;
  kmp[0] = 0;
  for (i = 1; i <= m; i++) {
    k = mp[i] = ba[i-1];
    while (k > 0 && (pattern[k] == pattern[i]))
        k = ba[k-1];
    kmp[i] = k;
  }
  return kmp;
}

/* --------------------------------------------------------------------------*/
void knuth_morris_pratt(int* kmp, char* text, int n, char* pattern, int m) {
  int i, j, match=1;

  for (i = 0; i < n ;i++)
    printf("%d", i%10);
  printf("\n");
  printf("%s\n", text);
  for (i = 0, j = 0; i  <= n - m + j;) {
    while(j < m && text[i] == pattern[j]) {
      printf("%c", pattern[j]);
      i++; j++;
    }
    if (j == m){ 
      printf("%*c  match on position %2d", n-i+1-match, ' ', i-m);
    }
    else{
      printf("%c", pattern[j]);    // print the mismatching symbol
    }
    match = 0;
    if (j==0)
      i++;
    else {
      j = kmp[j];
    }
    if (i - j <= n - m) printf("\n%*c%.*s", i-j, ' ', j, fillers);
  }
  printf("\n");
}

/* --------------------------------------------------------------------------*/
void Knuth_Morris_Pratt(int* kmp, char* text, int n, char* pattern, int m) {
  int i, j;

  for (i = 0, j = 0; i  <= n - m + j;) {
    while(j < m && text[i] == pattern[j]){
      i++; j++;
    }
    if (j == m) 
      printf("  match on position %2d\n", i-m);
    if (j==0)
      i++;
    else 
      j = kmp[j];
  }
}

/* --------------------------------------------------------------------------*/
void print_table(int* ba, int* mp,  int* kmp, char* str, int m) {
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

  printf("\nMP:      ");
  for (i = 0; i <= m; i++)
    printf("%2d ", mp[i]);

  printf("\nKMP:     ");
  for (i = 0; i <= m; i++)
    printf("%2d ", kmp[i]);
  printf("\n");  
}

// function main -------------------------------------------------------------
int main(int argc, char **argv) {
  int *ba, *mp, *kmp;
  int m, n;

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

  ba = border_array(argv[2], m);
  mp = mp_function(argv[2], ba, m);
  kmp = kmp_function(argv[2], ba, m);
  printf("Preprocessing:\n");
  print_table(ba, mp, kmp, argv[2], m);
  printf("\nProcessing MP:\n");
  knuth_morris_pratt(mp, argv[1], n, argv[2], m);
  printf("\nProcessing KMP:\n");
  knuth_morris_pratt(kmp, argv[1], n, argv[2], m);
  Knuth_Morris_Pratt(kmp, argv[1], n, argv[2], m);

  free(ba);
  free(mp);
  free(kmp);

  return EXIT_SUCCESS;
}
