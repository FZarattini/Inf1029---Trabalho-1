#include <stdio.h>
#include "matrix_lib.h"


int main(int argc, char *argv[])
{
  int resultado = 1;
  int i;
  float buffer;
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
  arq2 = argv[7];
  arq3 = argv[8];
  arq4 = argv[9];

  FILE* input1 = fopen(arq1, "rb");
  FILE* input2 = fopen(arq2, "rb");
  FILE* output1 = fopen(arq3, "wb");
  FILE* output2 = fopen(arq4, "wb");


  if(input1 == NULL || input2 == NULL || output1 == NULL || output2 == NULL){
    printf("Erro ao abrir arquivos. Abortando...\n");
    return 0;
  }

  MATRIX_TYPE *A = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));
  MATRIX_TYPE *B = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));
  MATRIX_TYPE *C = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));


   if ((A == NULL) || (C == NULL) || (C == NULL)) {
	printf("Problema em alocar memória das matrizes.\n");
	return 1;
  }

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

  i = 0;

  while(i < hA * wA){
  	int nread = fread(&buffer, sizeof(float), 1, input1);

  	if(nread < 1){
  		break;
  	}
  	  	A->rows[i] = buffer;
  	i++;
  }

  ptr = B->rows;
  i = 0;

  while(i < hB * wB){

  	fread(ptr, sizeof(float), 1, input2);
  	ptr += 1;
    i++;
  }
  
  resultado = scalar_matrix_mult(scalar, A);

  
  if(resultado == 0){
    printf("Erro ao multiplicar matriz por escalar!\n");
  }

  resultado = matrix_matrix_mult(A, B, C);

  printf("Gerando arquivo resultado - Multiplicacao matriz x escalar...\n");

  ptr = A->rows;
  i = 0;
  while( i < hA*wA){
    fwrite(ptr, sizeof(float), 1, output1);
    ptr+=1;
    i++;
  }

  printf("Arquivo de saida gerado com sucesso!\n");

  printf("Gerando arquivo resultado - Multiplicacao matrix x matriz...\n");
  
  ptr = C->rows;
  i = 0;
  while(i < hA * wB){
    fwrite(ptr, sizeof(float), 1, output2);
    ptr+=1;
    i++;
  }

  printf("Arquivo de saida gerado com sucesso!\n");

  for(i = 0; i< hA * wB; i++)
  {
	  printf("Resultado final: %.2f ------- posicao %d\n", C->rows[i], i);
  }


  if(resultado == 0){
    printf("Erro ao multiplicar matriz por matriz!\n");
  }

  free(A);
  free(B);
  free(C);

	return 0;
}
