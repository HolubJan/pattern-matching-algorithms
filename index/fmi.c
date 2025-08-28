#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TITLE  "FM-Index algorithm\n"
#define AUTHOR "Jan Holub, 2023\n"
#define HRULE  "----------------------------\n"
#define USAGE  "Usage: fmi <TEXT> <PATTERN>\n"

char fillers[1024];

typedef struct FMI {
  char *L, alphabet[256], n;
  int *C, alphabet_size, rev_alphabet[256];
} FMI_type;

/* --------------------------------------------------------------------------*/
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
int* suffix_array_naive(char* str, int n) {
  int i, b, c, *sa;

  sa = (int*) malloc ((n+1) * sizeof(int));
  for (i = 0; i < n; i++)
    sa[i]=i;
  do {
    b=0;
    for (i = 1; i < n; i++)
      //      if (strcmp(str+sa[i-1], str+sa[i]) > 0){
      if (suffcmp(str, n, sa[i-1], sa[i]) > 0){
	b=1;
	c=sa[i];
	sa[i]=sa[i-1];
	sa[i-1]=c;
      };
  } while (b==1);
  
  /*
  for (i = 0; i < n; i++){  // 
    printf(" %2d %2d  %s\n",i,sa[i],str+sa[i]);
  }
  */
    
  return sa;
}

/* --------------------------------------------------------------------------*/
FMI_type* fmi_construct(int* sa, char *text, int n) {
  int i, j, asize;
  char c;
  FMI_type* fmi;
  
  fmi = (FMI_type*) malloc (sizeof(FMI_type));
  fmi->n=n;
  for (asize=1, i = 1; i < n; i++)
    if (text[sa[i]]!=text[sa[i-1]]) asize++;
  fmi->alphabet_size=asize;
  fmi->C = (int*) malloc ((fmi->alphabet_size+1) * sizeof(int));
  c=text[sa[0]];             // the first character
  asize=1;             // alphabet size
  fmi->alphabet[0]=c;  // the first alphabet symbol
  fmi->rev_alphabet[c]=0;  // the first alphabet symbol
  fmi->C[asize-1]=0;   // position of symbol c in vector F
  for (i = 1; i < n; i++)
    if(c!=text[sa[i]]) {
      c=text[sa[i]];             // next character in the alphabet
      fmi->alphabet[asize]=c;        // next alphabet symbol
      fmi->rev_alphabet[c]=asize;  // the first alphabet symbol
      fmi->C[asize]=i;
      asize++;                   // alphabet size
    };
  fmi->L = (char*) malloc ((n+1) * sizeof(char));
  for (i = 0; i < n ;i++)
    fmi->L[i]=text[(sa[i]<1 ? n-1 : (sa[i]-1))];
  return fmi;
}

/* --------------------------------------------------------------------------*/
int occ_naive(char *text, int n, char c, int i) {
  int count=0, j;

  //  printf("[occ(%c, %d)=",c,i);
  j=i;
  while (i>=0) {
    if (text[i]==c) count++;
    i--;
  }
  //  printf("%d]",count);
  return count;
}

/* --------------------------------------------------------------------------*/
void fmi_search(int* sa, FMI_type* fmi, char *text, int n, char *pattern, int m) {
  int j, sp, ep;
  char c;

  j = m-1;
  sp = 0;
  ep = n-1;

  printf("sp = %2d, ep = %2d\n",sp,ep);
  do {
    c=pattern[j];
    sp = fmi->C[fmi->rev_alphabet[pattern[j]]]+occ_naive(fmi->L,n,c,sp-1);
    ep = fmi->C[fmi->rev_alphabet[pattern[j]]]+occ_naive(fmi->L,n,c,ep)-1;
    printf("sp = %2d, ep = %2d, P[%2d..%2d] = %s\t\tocc = %2d\n",sp,ep,j,m-1,pattern+j, ep-sp+1);
    j--;
  } while (j>=0);
  
  printf("Occurrences:\n");  
  while(sp<=ep){
    printf("i = %2d, sa[i] = %2d, text[sa[i]]= %s\n",sp,sa[sp],text + sa[sp]);
    sp++;
  }
  printf("\n");  
}

/* --------------------------------------------------------------------------*/
void print_table(int* sa, FMI_type* fmi, char* str, int n) {
  int i;
  
  printf("\nIndex:  ");
  for (i = 0; i < n; i++)
    printf(" %2d", i);

  printf("\nString:  ");
  for (i = 0; i < n; i++)
    printf("%2c ", str[i]);

  printf("\nSA[i]:   ");
  for (i = 0; i < n; i++)
    printf("%2d ", sa[i]);
  printf("\nSA[i]-1  ");
  for (i = 0; i < n; i++)
    printf("%2d ", (sa[i]<1 ? n-1 : (sa[i]-1)));
  printf("\nF:       ");
  for (i = 0; i < n; i++)
    printf("%2c ", str[sa[i]]);
  printf("\nL:       ");
  for (i = 0; i < n; i++)
    printf("%2c ", fmi->L[i]);
  printf("\n");  
  printf("\nC:       ");
  for (i = 0; i < fmi->alphabet_size; i++)
    printf("%2c ", fmi->alphabet[i]);
  printf("\n         ");
  for (i = 0; i < fmi->alphabet_size; i++)
    printf("%2d ", fmi->C[i]);

  printf("\n");  
}

// function main -------------------------------------------------------------
int main(int argc, char **argv) {
  int *sa, *C;
  char *L;
  int m, n;
  FMI_type* fmi;

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

  printf("Preprocessing:\n");
  sa = suffix_array_naive(argv[1], n);
  fmi = fmi_construct(sa, argv[1], n);
  print_table(sa, fmi, argv[1], n);
  printf("\nProcessing FMI:\n");
  fmi_search(sa, fmi,  argv[1], n, argv[2], m);
  free(fmi);
  return EXIT_SUCCESS;
}
