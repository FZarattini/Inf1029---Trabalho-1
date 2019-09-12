#include <stdio.h>
#include "matrix_lib.h"


int main(int argc, char *argv[])
{
  int resultado = 1;
  int i;
  long unsigned int hA = 1<<21;
  long unsigned int wA = 1<<21;
  long unsigned int hB = 1<<21;
  long unsigned int wB = 1<<21;
  float scalar;
  char* arq1;
  char* arq2;
  char* arq3;
  char* arq4;
  float* ptr;

  char *eptr = NULL;

  FILE* input1 = fopen(arq1, "rb");
  FILE* input2 = fopen(arq2, "rb");
  FILE* output1 = fopen(arq3, "wb");
  FILE* output2 = fopen(arq4, "wb");

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

  ptr = A->rows;

  while(!feof(input1)){
  	fread(ptr, sizeof(float), 1, input1);
  	ptr += 1;
  }

  ptr = B->rows;

  while(!feof(input2)){
  	fread(ptr, sizeof(float), 1, input2);
  	ptr += 1;
  }

  for(i = 0; i < hA*wA; i++){
  	printf("%.2f\n", A->rows[i]);
  }

  for(i = 0; i < hA*wA; i++){
  	printf("%.2f\n", B->rows[i]);
  }
  /*
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
  }*/

	return 0;
}