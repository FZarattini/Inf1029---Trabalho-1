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

  printf("Escalar %f\n", scalar);

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

  MATRIX_TYPE *A = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));
  MATRIX_TYPE *B = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));
  MATRIX_TYPE *C = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));

  A->height = hA;
  A->width = wA;
  B->height = hB;
  B->width = wB;
  C->height = hA;
  C->width = wB;

  //printf("Antes Matrix->rows\n");

  A->rows = (float*)aligned_alloc(32, hA*wA*sizeof(float));
  B->rows = (float*)aligned_alloc(32, hB*wB*sizeof(float));
  C->rows = (float*)aligned_alloc(32, hA*wB*sizeof(float));

  //printf("Pre prt = A->rows\n");

  ptr = A->rows;

  //printf("VAI LER ARQUIVO!\n");
  i = 0;
  /*while(fread(ptr, sizeof(float), 1, input1) != 0){
  	printf("aaaaaaaaaaa\n");
  	//ptr += 1;
  }*/
  while(1){

  	//printf("VAI LER ARQUIVO A!\n");
  	int nread = fread(&buffer, sizeof(float), 1, input1);

  	if(nread < 1){
  		break;
  	}
  	  	//printf("vai salvar no vetor! A %d\n", i);
  	  	A->rows[i] = buffer;
  	i++;
  }

  printf("LEU ARQUIVO!\n");

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
  
  
  //Chamando as funções
  printf("%d\n", resultado);
  resultado = scalar_matrix_mult(scalar, A);
  
  if(resultado == 0){
    printf("Erro ao multiplicar matriz por escalar!\n");
  }

  printf("%d\n", resultado);

  for(i = 0; i< wA*hA; i++)
  {

	  printf("%f\n", A->rows[i]);

  }


  resultado = matrix_matrix_mult(A, B, C);

  for(i = 0; i< wA*hA; i++)
  {

	  printf("%f\n", C->rows[i]);

  }


  if(resultado == 0){
    printf("Erro ao multiplicar matriz por matriz!\n");
  }

	return 0;
}