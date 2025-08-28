#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MIN(x,y)      (((x) < (y)) ? (x) : (y))
#define MAX(x,y)      (((x) > (y)) ? (x) : (y))
#define MIN3(x,y,z) (((x) < MIN(y,z)) ? (x) : MIN(y,z))
#define MAX3(x,y,z) (((x) < MAX(y,z)) ? (x) : MAX(y,z))
#define MIN4(w,x,y,z) ((MIN(w,x) < MIN(y,z)) ? (MIN(w,x)) : MIN(y,z))
#define MAX4(w,x,y,z) ((MAX(w,x) < MAX(y,z)) ? (MAX(w,x)) : MAX(y,z))

#define ALPHABET_SIZE 256
#define MIN_CHAR 'a'
#define MAX_CHAR 'd'
#define ANY_OTHER_SYMBOL '?'

#define TITLE  "Approximate string matching - bit parallelism\n"
#define AUTHOR "Jan Holub, 2023\n"
#define HRULE  "-------------------------------------------------\n"
#define USAGE  "Usage: approximate_matching-bp {h|l|d} <K> <PATTERN> <TEXT>\n"

/* --------------------------------------------------------------------------*/
void print_table(char* p, int m, int k, char *t, int n, unsigned long *D, unsigned long **R){
  int i,j,l,asize;
  unsigned char c;
  unsigned long mask=1l;
  char is_used[ALPHABET_SIZE];
  unsigned char used_alphabet[ALPHABET_SIZE];
  int used_alphabet_size, rev_alphabet[ALPHABET_SIZE];


    // compute the alphabet used 
  for (i=0; i<ALPHABET_SIZE; i++)
    is_used[i]=0;
  for (j=0; j<m; j++)
    is_used[p[j]]=1;
  for (asize=0, i=0; i<ALPHABET_SIZE; i++) {
    if (is_used[i]==1){
      rev_alphabet[i]=asize;
      used_alphabet[asize]=i;
      asize++;
    };
  }
  used_alphabet[asize]=ALPHABET_SIZE-1; // a representative of a symbol not in p
  
    // print table D
  printf("D    : ");
  for (i=0; i<asize; i++){
    printf("%2c ",used_alphabet[i]);
  }
  printf("%2c ",ANY_OTHER_SYMBOL);
  printf("\n");
  for (j=0; j<m; j++, mask<<=1){
    printf("%2d %c :",j,p[j]);
    for (i=0; i<=asize; i++){
      //      printf(" %2lu",D[c]);
      printf(" %2d",((D[used_alphabet[i]]&mask)==0 ? 0 : 1));
    }
    printf("\n");
  }
  printf("\n");
  
    // print tables R
  for (l=0; l<=k; l++) {
    printf("R_%d  : ",l);
    for (i=0; i<=n; i++){
      printf("%2d ",i);
    }
    printf("\n");
    printf("     :  - ");
    for (i=0; i<n; i++){
      printf(" %c ",t[i]);
    }
    printf("\n");
    for (mask=1l, j=0; j<m; j++, mask<<=1){
      printf("%2d %c :",j,p[j]);
      for (i=0; i<=n; i++){
	printf(" %2u",((R[l][i]&mask)==0 ? 0 : 1));
      }
      printf("\n");
    }
    printf("\n");
  }
}

/* --------------------------------------------------------------------------*/
void compute_mask_vectors(unsigned char* p, int m, unsigned long *D) {
  int i,j;
  unsigned char c;
  unsigned long mask=1l;

  for (i=0; i<ALPHABET_SIZE; i++){
    D[i]=0-1;
  }
  for (j=0; j<m; j++, mask<<=1){
    D[p[j]]-=mask;
  }
}

/* --------------------------------------------------------------------------*/
int table_test(char* p, int m, int k, char *t, int n, unsigned long *D, unsigned long **R) {
  int i,l;
  unsigned long count=0l;

  for (l=0; l<=k; l++){
    for (i=0; i<=n; i++){
      R[l][i]=count++;
    }
  }
  return -2000;
}

/* --------------------------------------------------------------------------*/
int compute_hamming_distance(char* p, int m, int k, char *t, int n, unsigned long *D, unsigned long **R) {
  int i,l,count=0,match=1l<<(m-1);

  for (l=0; l<=k; l++)       // initial value of the vectors
    R[l][0]=0l-1;
  for (i=1; i<=n; i++){
    for (l=0; l<=k; l++){
      R[l][i]= (R[l][i-1]<<1)|D[t[i-1]]; // match
      if (l>0)
	R[l][i] &= (R[l-1][i-1]<<1);     // replace
    }
    if ((R[k][i]&match)==0){
	printf("An occurrence at position: %2d\n",i-1);
	count++;
      }
  }
  return count;
}

/* --------------------------------------------------------------------------*/
int compute_levenshtein_distance(char* p, int m, int k, char *t, int n, unsigned long *D, unsigned long **R) {
  int i,l,count=0;
  unsigned long match=1l<<(m-1), r_init;

  for (r_init=0l-1, l=0; l<=k; l++, r_init<<=1) // initial value of vectors R
    R[l][0]=r_init;

  for (i=1; i<=n; i++){
    for (l=0; l<=k; l++){
      R[l][i]= (R[l][i-1]<<1)|D[t[i-1]]; // match
      if (l>0)
	R[l][i] &= (R[l-1][i-1]<<1) & (R[l-1][i-1] & (R[l-1][i]<<1));     // replace & insert & delete
    }
    if ((R[k][i]&match)==0){
	printf("An occurrence at position: %2d\n",i-1);
	count++;
      }
  }
  return count;
}

/* --------------------------------------------------------------------------*/
  int compute_damerau_distance(char* p, int m, int k, char *t, int n, unsigned long *D, unsigned long **R) {
  int i,l,count=0;
  unsigned long **S,RS;
  unsigned long match=1l<<(m-1), r_init;

                                                          // auxilary vectors S
  S = (unsigned long**) malloc ((k) * sizeof(unsigned long*));
  for (l=0; l<k; l++)
    S[l] = (unsigned long*) malloc ((n+1) * sizeof(unsigned long));
  
  for (r_init=0l-1, l=0; l<=k; l++, r_init<<=1)   // initial value of vectors R
    R[l][0]=r_init;
  for (r_init=0l-1, l=0; l<k; l++, r_init<<=1)    // initial value of vectors S
    S[l][0]=r_init;
 
  for (i=1; i<=n; i++){
    for (l=0; l<=k; l++){
      //      R[l][i]= (R[l][i-1]<<1)|D[t[i-1]];          // match
      R[l][i]= (R[l][i-1]<<1);
      R[l][i]|=D[t[i-1]];          // match
      if (l<k)
	S[l][i]= (R[l][i-1]<<1)|(D[t[i-1]]>>1);
      if (l>0)
	R[l][i] &= (R[l-1][i-1]<<1) & (R[l-1][i-1]) & (R[l-1][i]<<1) & ((S[l-1][i-1]|D[t[i-1]])<<1);     // replace & insert & delete & transpose
    }
    if ((R[k][i]&match)==0){
	printf("An occurrence at position: %2d\n",i-1);
	count++;
      }
  }
  for (l=0; l<=k; l++)
    free(S[l]);
  free(S);
  return count;
}

// function main -------------------------------------------------------------
int main(int argc, char **argv) {
  int *d;
  char *p, *t, dtype;
  int n, m, result, k, l;
  unsigned int i;
  unsigned long D[ALPHABET_SIZE], **R;

  printf(HRULE);
  printf(TITLE);
  printf(AUTHOR);
  printf(HRULE);
  if (argc < 5) {
    printf(USAGE);
    return EXIT_FAILURE;
  }
  dtype=argv[1][0];
  k=atoi(argv[2]);
  p=argv[3]; 
  t=argv[4];
  printf("distance=");
  switch(dtype){
  case 'h': printf("Hamming\n");
    break;
  case 'l': printf("Levenshtein\n");
     break;
  case 'd': printf("Damerau\n");
     break;
  case 't': printf("table test\n");
     break;
  default: printf("unknown\n");
   return EXIT_FAILURE;
  }   
  printf("limit k=%d\n",k);
  m = strlen(argv[3]);
  n = strlen(argv[4]);
  printf("p=%s\n m=%d\n", p, m);
  printf("t=%s\n n=%d\n", t, n);
  if(m>sizeof(long)*8){
    printf("The computer word (long, %lu bits) cannot accommodate pattern of length %d.\n",sizeof(long)*8,n);
   return EXIT_FAILURE;
  }

  compute_mask_vectors(p,m,D);
  //  printf("The computer word: (unsigned long, %lu bits), (unsigned long long,  %lu bits).\n",sizeof(unsigned long)*8,sizeof(unsigned long long)*8);

  R = (unsigned long**) malloc ((k+1) * sizeof(unsigned long*));
  for (l=0; l<=k; l++)
    R[l] = (unsigned long*) malloc ((n+1) * sizeof(unsigned long));
  switch(dtype){
  case 'h': result = compute_hamming_distance(p,m,k,t,n,D,R);
    break;
  case 't': result = table_test(p,m,k,t,n,D,R);
    break;
  case 'l': result = compute_levenshtein_distance(p,m,k,t,n,D,R);
     break;
  case 'd': result = compute_damerau_distance(p,m,k,t,n,D,R); 
     break;
  default: printf("unknown\n");
   return EXIT_FAILURE;
  }   
  
  print_table(p,m,k,t,n,D,R); 
  printf("The number of occurrences of p in t %d.\n", result);
  for (l=0; l<=k; l++)
    free(R[l]);
  free(R);
  return EXIT_SUCCESS;
}
