#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TITLE  "Duval's algorithm for Lyndon decomposition\n"
#define AUTHOR "Jan Holub, 2024\n"
#define HRULE  "------------------------------------------\n"
#define USAGE  "Usage: duval <TEXT>\n"

int* duval(char* x, int n) {
  int i, j, k, l;
  int* ld = (int*) malloc (n * sizeof(int));

  i = l = 0;        // l - iterator in the output vector  i j k
  while (i < n) {
    k=i; j=i+1;
    while(j<n && x[k]<=x[j]) {
      if (x[k] < x[j])
        k=i;                       // switch to a next rotation
      else
        k++;                            // extend this rotation
      j++;
    }
    while (i<=k){
      i+=(j-k);
      ld[l++] = i;                        // store the position
    }
  }
  return ld;
}

void print_results(int* ld, char* str, int size) {
  int i, j;

  printf("Length: %u\n", size);
  printf("String: %s\n", str);
  printf("Lyndon: ");
  for (i = 0, j = 0; i < size; i++) {
    printf("%c", str[i]);
    if (i == (ld[j] - 1)) {
      printf("|");
      j++;
    }
  }
  printf("\n");
}

int main (int argc, char *argv[]) {
  int size, *ld;

  if (argc < 2) return EXIT_FAILURE;

  size = strlen(argv[1]);
  ld = duval(argv[1], size);
  print_results(ld, argv[1], size);
  free(ld);
  return EXIT_SUCCESS;
}
