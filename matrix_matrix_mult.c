#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
//#include "timer.h"

typedef struct matrix{
  unsigned long int height;
  unsigned long int width;
  float *rows;
}MATRIX_TYPE;

int matrix_matrix_mult(struct matrix *matrixA, struct matrix * matrixB, struct matrix * matrixC){
  long unsigned int hA = 1<<21;
  long unsigned int wA = 1<<21;
  long unsigned int hB = 1<<21;
  long unsigned int wB = 1<<21;
  long unsigned int i, ind, index;

  hA = matrixA->height;
  hB = matrixB->height;
  wA = matrixA->width;
  wB = matrixB->width;

  matrixA->rows =  (float*)aligned_alloc(32, hA*wA*sizeof(float));
  matrixB->rows = (float*)aligned_alloc(32, hB*wB*sizeof(float));
  matrixC->rows = (float*)aligned_alloc(32, hA*wB*sizeof(float));
  matrixC->height = hA;
  matrixC->width = wB;
  float *valueA_alligned = (float*) aligned_alloc(32, wB * sizeof(float));

  /* Initialize the three argument vectors */
  __m256 vec_rowsA = _mm256_set1_ps(5.0f);
  __m256 vec_rowsB = _mm256_set1_ps(2.0f);
  __m256 vec_result = _mm256_set1_ps(0.0f);

  float* nxt_rowsA = matrixA->rows; 
  float* nxt_rowsB = matrixB->rows; 
  float* nxt_result = matrixC->rows;
  float* nxt_valueA = valueA_alligned;
  float* start_rowsA = matrixA->rows;
  float* start_rowsB = matrixB->rows;

  for (i = 0; 
	i < (hA * wA); 
	i += 8, nxt_rowsA += 8, nxt_valueA+=8) {

	  /* Store the elements of the vectors in the arrays */
	  _mm256_store_ps(nxt_rowsA, vec_rowsA);
	  _mm256_store_ps(nxt_valueA, vec_rowsA);
  }

  for(i = 0; i < (hB * wB); i += 8, nxt_rowsB += 8 ){
    _mm256_store_ps(nxt_rowsB, vec_rowsB);
  }


  for(i = 0; i < (hA * wB); i += 8, nxt_rowsB += 8 ){
    _mm256_store_ps(nxt_result,vec_result);
  }

  /* Compute the difference between the two vectors */
  nxt_rowsA = matrixA->rows; 
  nxt_rowsB = matrixB->rows; 
  nxt_result = matrixC->rows;
  nxt_valueA = valueA_alligned;
  
  float *start_result = matrixC->rows;

  for( i = 0; i < (hA * wA); i += 8, nxt_rowsA += 8){

    __m256 vec_valueA = _mm256_set1_ps(*nxt_rowsA);

    if(i % wB == 0){
        nxt_rowsB = start_rowsB;
        nxt_result = start_result;
    }

    for(ind = 0 ; ind < (hB*wB); ind += 8, nxt_rowsB += 8){
      __m256 vec_rowsB = _mm256_load_ps(nxt_rowsB);

      for(index = 0 ; index < (hA * wB); index += 8, nxt_result += 8){
       
        __m256 vec_result = _mm256_load_ps(nxt_result);

        vec_result = _mm256_fmadd_ps(vec_valueA, vec_rowsB, vec_result);

       _mm256_store_ps(nxt_result, vec_result);

      }
    }  

  }

  for(int index = 0 ; index < (hA*wB); index++){
    printf("%.2f  -----  %d\n", matrixC->rows[index], index);

  }

  return 1;
}

int main(int argc, char *argv[]) {
  long unsigned int hA = 1<<21;
  long unsigned int wA = 1<<21;
  long unsigned int hB = 1<<21;
  long unsigned int wB = 1<<21;
  int resultado;
  char *eptr = NULL;

  // Convert arguments
  hA = strtol(argv[1], &eptr, 10);
  wA = strtol(argv[2], &eptr, 10);
  hB = strtol(argv[3], &eptr, 10);
  wB = strtol(argv[4], &eptr, 10);

  printf("MALOCANDO\n");
  MATRIX_TYPE *A = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));
  MATRIX_TYPE *B = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));
  MATRIX_TYPE *C = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));
  printf("MALOCOU\n");

  if(wA != hB){
    printf("Nao é possivel multiplicar as duas matrizes!\n");
	return 0;
  }

  /* Iitialize the two matrices and the result matrix */
  A->height = hA;
  A->width = wA;
  B->height = hB;
  B->width = wB;

  resultado = matrix_matrix_mult(A, B, C);

  if(resultado == 1){
    printf("Sucesso!\n");
  }else{
    printf("Erro de calculo!\n");
  }

  return 0;
}
