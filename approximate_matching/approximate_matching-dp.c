#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MIN(x,y)      (((x) < (y)) ? (x) : (y))
#define MAX(x,y)      (((x) > (y)) ? (x) : (y))
#define MIN3(x,y,z) (((x) < MIN(y,z)) ? (x) : MIN(y,z))
#define MAX3(x,y,z) (((x) < MAX(y,z)) ? (x) : MAX(y,z))
#define MIN4(w,x,y,z) ((MIN(w,x) < MIN(y,z)) ? (MIN(w,x)) : MIN(y,z))
#define MAX4(w,x,y,z) ((MAX(w,x) < MAX(y,z)) ? (MAX(w,x)) : MAX(y,z))
#define DP(x,y) (D[(x)+(y)*(m+1)])
/*
  columnwise: row x u(), column y
     :  0  1  2  3 
     :  -  a  c  d 
 0 - :  0  3  6  9
 1 a :  1  4  7 10
 2 b :  2  5  8 11
*/

#define TITLE  "Approximate string matching - dynamic programming\n"
#define AUTHOR "Jan Holub, 2023\n"
#define HRULE  "-------------------------------------------------\n"
#define USAGE  "Usage: approximate_matching-dp {h|l|d} <K> <PATTERN> <TEXT>\n"

/* --------------------------------------------------------------------------*/
void print_table(char* p, int m, int k, char *t, int n, int *D){
  int i,j;

  printf("     : ");
  for (i=0; i<=n; i++){
    printf("%2d ",i);
  }
  printf("\n");
  printf("     :  - ");
  for (i=0; i<n; i++){
    printf(" %c ",t[i]);
  }
  printf("\n");
  for (j=0; j<=m; j++){
    if (j==0)
      printf("%2d - :",j);
    else
      printf("%2d %c :",j,p[j-1]);
    for (i=0; i<=n; i++){
      printf(" %2d",DP(j,i));
    }
    printf("\n");
  }
}

/* --------------------------------------------------------------------------*/
int table_test(char* p, int m, int k, char *t, int n, int *D) {
  int i,j,count=0;

  for (i=0; i<=n; i++){
    for (j=0; j<=m; j++){
      DP(j,i)=count++;
    }
  }
  return -2000;
}

/* --------------------------------------------------------------------------*/
int compute_hamming_distance(char* p, int m, int k, char *t, int n,int *D) {
  int i,j,count=0;

  for (j=1; j<=m; j++)
    DP(j,0)=k+1;
  for (i=0; i<=n; i++)
    DP(0,i)=0;
   
  for (i=1; i<=n; i++){
    for (j=1; j<=m; j++){
      DP(j,i)=DP(j-1,i-1)+((p[j-1]==t[i-1]) ? 0: 1);
      if(j==m && DP(j,i)<=k){
	//	printf("An occurrence at position: %2d (DP[%2d.%2d]=%d)\n",i-m+1,j,i,DP(j,i));
	count++;
      }
    }
  }
  return count;
}

/* --------------------------------------------------------------------------*/
int compute_levenshtein_distance(char* p, int m, int k, char *t, int n,int *D) {
  int i,j,count=0;

  for (j=1; j<=m; j++)
    DP(j,0)=j;
  for (i=0; i<=n; i++)
    DP(0,i)=0;
   
  for (i=1; i<=n; i++){
    for (j=1; j<=m; j++){
      DP(j,i)=MIN3(DP(j-1,i-1)+((p[j-1]==t[i-1]) ? 0: 1),DP(j-1,i)+1,DP(j,i-1)+1);
      if(j==m && DP(j,i)<=k){
	count++;
      }
    }
  }
  return count;
}

/* --------------------------------------------------------------------------*/
int compute_damerau_distance(char* p, int m, int k, char *t, int n,int *D) {
  int i,j,count=0;

  for (j=1; j<=m; j++)
    DP(j,0)=j;
  for (i=0; i<=n; i++)
    DP(0,i)=0;
   
  for (i=1; i<=n; i++){
    for (j=1; j<=m; j++){
      DP(j,i)=MIN3(DP(j-1,i-1)+((p[j-1]==t[i-1]) ? 0: 1),DP(j-1,i)+1,DP(j,i-1)+1);
      if (i>1 && j>1 && p[j-2]==t[i-1] && p[j-1]==t[i-2])
	DP(j,i)=MIN(DP(j,i),DP(j-2,i-2)+1);
       if(j==m && DP(j,i)<=k){
	count++;
      }
    }
  }
  return count;
}

// function main -------------------------------------------------------------
int main(int argc, char **argv) {
  int *D;
  char *p, *t, dtype;
  int n, m, result, k;

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
  printf("p=%s\n", p);
  printf("t=%s\n", t);
  m = strlen(argv[3]);
  n = strlen(argv[4]);
  D = (int*) malloc ((m+1) * (n+1) * sizeof(int));
  switch(dtype){
  case 'h': result = compute_hamming_distance(p,m,k,t,n,D);
    break;
  case 't': result = table_test(p,m,k,t,n,D);
     break;
  case 'l': result = compute_levenshtein_distance(p,m,k,t,n,D);
     break;
  case 'd': result = compute_damerau_distance(p,m,k,t,n,D); 
     break;
  default: printf("unknown\n");
   return EXIT_FAILURE;
  }   
  print_table(p,m,k,t,n,D); 
  printf("The number of occurrences of p in t %d.\n", result);
  free(D);
  return EXIT_SUCCESS;
}
