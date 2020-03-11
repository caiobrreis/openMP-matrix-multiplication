#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int get_size(FILE *fp) { 
  int n = 1;
  char c;

  while(!feof(fp)) {
    c = fgetc(fp);
    if(c == '\n')
      n++;
  }
  return n;
}

int **loc_matrix(int n) {
  
  int **matrix = malloc(n*sizeof(int*));

  for(int i = 0; i < n; i++){
    matrix[i] = malloc(n*sizeof(int));
  }

  return matrix;
}

int **get_values(int n, FILE *fp) {
  int **matrix = loc_matrix(n); 

  for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++)
      fscanf(fp,"%d",&matrix[i][j]); 

  return matrix; 
}


int main() {
  int i,j,k;
  int **A;
  int **B;
  int **C;
  FILE *arq;

  arq = fopen("matrizA.txt","r");
  if(arq == NULL)
    printf("no such file");
  
  int n = get_size(arq); 
  rewind(arq); 
  A = get_values(n, arq); 
  fclose(arq);
 
  arq = fopen("matrizB.txt","r"); 
  if(arq == NULL)
    printf("no such file");
  
  B = get_values(n, arq); 
  fclose(arq);

  C = loc_matrix(n);

 // Multiplication
  #pragma omp parallel for
  for(i = 0; i < n; i++) {
    #pragma omp parallel for
    for(j = 0; j < n; j++) {
      C[i][j] = 0;
      #pragma omp parallel for
      for(k = 0; k < n; k++)
        C[i][j] += A[i][k]*B[k][j];
    }
  }

  arq = fopen("result.txt","w");
  if(arq == NULL)
	 printf("no such file");
  
  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++)
      fprintf(arq,"%d ", C[i][j]);
    fprintf(arq,"\n");
  }
  fclose(arq);
  
  return 0;
}