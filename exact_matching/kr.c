#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define TITLE  "Karp-Rabin algorithm\n"
#define AUTHOR "Jan Holub, 2023\n"
#define HRULE  "-------------------------------\n"
#define USAGE  "Usage: kr <TEXT> <PATTERN>\n"

char fillers[1024];
char spaces[1024];

#define BASE 5
#define MODULE 8

/* --------------------------------------------------------------------------*/
int get_hash(char *pattern, int size) {
  int i, result = 0;

  for (i = 0; i < size; i++)
    result += (pattern[i] - 'a')*pow(BASE, size - i - 1);
  return result % MODULE;
}

/* --------------------------------------------------------------------------*/
int roll_hash(int hash, char prev, char next, int size) {
  return ((int)((hash - (prev-'a')*pow(BASE, size-1))*BASE + (next-'a'))%MODULE + MODULE)%MODULE;
}

/* --------------------------------------------------------------------------*/
void karp_rabin(char* text, int n, char* pattern, int m) {
  int i, j;
  int t_hash, p_hash;
  
  //  printf("Preprocessing:\n");
  t_hash = get_hash(text, m);
  p_hash = get_hash(pattern, m);

  printf("\nProcessing:\n");
  for (i = 0; i < n ;i++)
    printf("%d", i%10);
  printf("\n");
  printf("%s\n", text);
  for (i = 0 ; i <= n - m; i++) {
    if (t_hash == p_hash){
      printf("%*.s%.*s", i, spaces, m, fillers);
      if (! strncmp(text + i, pattern, m))
	printf("%*c  match on position %2d\n", n-i, ' ', i);
      else
	printf("\n");
    }
    t_hash = roll_hash(t_hash, text[i], text[m+i], m);
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
  memset(fillers, (int)'.', 1024);
  memset(spaces, (int)' ', 1024);

  printf("T=%s\n", argv[1]);
  printf("P=%s\n", argv[2]);
  n = strlen(argv[1]);
  m = strlen(argv[2]);
  karp_rabin(argv[1], n, argv[2], m);

  return EXIT_SUCCESS;
}
