#include <stdio.h>
#include "matrix_lub.h"


int main(int argc, char *argv[])
{
  int resultado = 1;
  long unsigned int hA = 1<<21;
  long unsigned int wA = 1<<21;
  long unsigned int hB = 1<<21;
  long unsigned int wB = 1<<21;
  float scalar;
  char* arq1;
  char* arq2;
  char* arq3;
  char* arq4;

  char *eptr = NULL;

  // Check arguments
  if (argc != 10) {
        printf("Usage: %s <vector_length>\n", argv[0]);
        return 0;
  } else {
        //printf("Number of args: %d\n", argc);
        //for (int i=0; i<argc; ++i)
         //       printf("argv[%d] = %s\n", i, argv[i]);
  }


  scalar = strtof(argv[1], NULL);
  hA = strtol(argv[2], &eptr, 10);
  wA = strtol(argv[3], &eptr, 10);
  hB = strtol(argv[4], &eptr, 10);
  wB = strtol(argv[5], &eptr, 10);

  arq1 = argv[6];
  arq1 = argv[7];
  arq1 = argv[8];
  arq1 = argv[9];

  MATRIX_TYPE *A = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));
  MATRIX_TYPE *B = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));
  MATRIX_TYPE *C = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));

  A->height = hA;
  A->width = wA;
  B->height = hB;
  B->width = wB;
  C->height = hA;
  C->width = wB;

  A->rows = (float*)aligned_alloc(32, hA*wA*sizeof(float));
  B->rows = (float*)aligned_alloc(32, hB*wB*sizeof(float));
  C->rows = (float*)aligned_alloc(32, hA*wB*sizeof(float));

  //Chamando as funções
  resultado = scalar_matrix_mult(scalar, A);
  
  if(resultado == 0){
    printf("Erro ao multiplicar matriz por escalar!\n");
    return 0;
  }

  resultado = matrix_matrix_mult(A, B, C);

  if(resultado == 0){
    printf("Erro ao multiplicar matriz por matriz!\n");
    return 0;
  }

	return 0;
}