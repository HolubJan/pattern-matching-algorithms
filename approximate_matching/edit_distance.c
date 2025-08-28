#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MIN(x,y)      (((x) < (y)) ? (x) : (y))
#define MAX(x,y)      (((x) > (y)) ? (x) : (y))
#define MIN3(x,y,z) (((x) < MIN(y,z)) ? (x) : MIN(y,z))
#define MAX3(x,y,z) (((x) < MAX(y,z)) ? (x) : MAX(y,z))
#define MIN4(w,x,y,z) ((MIN(w,x) < MIN(y,z)) ? (MIN(w,x)) : MIN(y,z))
#define MAX4(w,x,y,z) ((MAX(w,x) < MAX(y,z)) ? (MAX(w,x)) : MAX(y,z))
#define DP(x,y) (D[(x)+(y)*(n+1)])
/*
  columnwise: row x u(), column y
     :  0  1  2  3 
     :  -  a  c  d 
 0 - :  0  3  6  9
 1 a :  1  4  7 10
 2 b :  2  5  8 11
*/

#define TITLE  "Computing edit distance\n"
#define AUTHOR "Jan Holub, 2023\n"
#define HRULE  "----------------------------\n"
#define USAGE  "Usage: edit_distance {h|l|d} <TEXT_1> <TEXT_2>\n"

/* --------------------------------------------------------------------------*/
int table_test(char* u, int n, char *v, int m, int *D) {
  int i,j,count=0;

  for (i=0; i<=m; i++){
    for (j=0; j<=n; j++){
      DP(j,i)=count++;
    }
  }
  return -2000;
}

/* --------------------------------------------------------------------------*/
int compute_hamming_distance(char* u, int n, char *v, int m, int *D) {
  int i,count=0;

  if (m!=n){
    printf("Hamming distance is defined on strings of the same length (%d and %d)!\n",n,m);
    exit(EXIT_FAILURE);
  }
 
  for (i=0; i<=m; i++){
    if(u[i]!=v[i]) count++;
  }
  return count;
}

/* --------------------------------------------------------------------------*/
int compute_levenshtein_distance(char* u, int n, char *v, int m, int *D) {
  int i,j,count=0;

  for (i=0; i<=n; i++)
    DP(i,0)=i;
  for (j=0; j<=m; j++)
    DP(0,j)=j;
   
  for (j=1; j<=m; j++){
    for (i=1; i<=n; i++){
      DP(i,j)=MIN3(DP(i-1,j-1)+((u[i-1]==v[j-1]) ? 0: 1),DP(i-1,j)+1,DP(i,j-1)+1);
      //      printf("DP(%2d,%2d) = MIN3( DP(%2d,%2d)+1, DP(%2d,%2d)+1, DP(%2d,%2d)+1 ) = min( %2d, %2d, %2d )\n",i,j,i-1,j-1,i-1,j,i,j-1,DP(i-1,j-1)+1,DP(i-1,j)+1,DP(i,j-1)+1);
    }
  }
  return DP(n,m);
}

/* --------------------------------------------------------------------------*/
int compute_damerau_distance(char* u, int n, char *v, int m, int *D) {
  int i,j,count=0;

  for (i=0; i<=n; i++)
    DP(i,0)=i;
  for (j=0; j<=m; j++)
    DP(0,j)=j;
   
  for (j=1; j<=m; j++){
    for (i=1; i<=n; i++){
      DP(i,j)=MIN3(DP(i-1,j-1)+((u[i-1]==v[j-1]) ? 0: 1),DP(i-1,j)+1,DP(i,j-1)+1);
      if (i>1 && j>1 && u[i-2]==v[j-1] && u[i-1]==v[j-2])
	DP(i,j)=MIN(DP(i,j),DP(i-2,j-2)+1);
    }
  }
  return DP(n,m);
}

/* --------------------------------------------------------------------------*/
void print_table(char* u, int n, char *v, int m, int *D){
  int i,j;

  printf("     : ");
  for (j=0; j<=m; j++){
    printf("%2d ",j);
  }
  printf("\n");
  printf("     :  - ");
  for (j=0; j<m; j++){
    printf(" %c ",v[j]);
  }
  printf("\n");
  for (i=0; i<=n; i++){
    if (i==0)
      printf("%2d - :",i);
    else
      printf("%2d %c :",i,u[i-1]);
    for (j=0; j<=m; j++){
      printf(" %2d",DP(i,j));
    }
    printf("\n");
  }
}

// function main -------------------------------------------------------------
int main(int argc, char **argv) {
  int *D;
  char *u, *v, dtype;
  int m, n, result;

  printf(HRULE);
  printf(TITLE);
  printf(AUTHOR);
  printf(HRULE);
  if (argc < 4) {
    printf(USAGE);
    return EXIT_FAILURE;
  }
  dtype=argv[1][0];
  u=argv[2]; 
  v=argv[3];
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
  printf("u=%s\n", u);
  printf("v=%s\n", v);
  n = strlen(argv[2]);
  m = strlen(argv[3]);
  D = (int*) malloc ((n+1) * (m+1) * sizeof(int));
  switch(dtype){
  case 'h': result = compute_hamming_distance(u,n,v,m,D);
    break;
  case 't': result = table_test(u,n,v,m,D);
     break;
  case 'l': result = compute_levenshtein_distance(u,n,v,m,D);
     break;
  case 'd': result = compute_damerau_distance(u,n,v,m,D); 
     break;
  default: printf("unknown\n");
   return EXIT_FAILURE;
  }   
  if (dtype!='h') print_table(u,n,v,m,D);
  printf("Edit distance between u and v is %d.\n", result);
  return EXIT_SUCCESS;
}
