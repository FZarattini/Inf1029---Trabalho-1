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

int main(int argc, char *argv[]) {
  long unsigned int hA = 1<<21;
  long unsigned int wA = 1<<21;
  long unsigned int hB = 1<<21;
  long unsigned int wB = 1<<21;
  long unsigned int i;
  long unsigned int ind;
  long unsigned int index;
  int cont, iter = 0;
  char *eptr = NULL;

  // Convert arguments
  hA = strtol(argv[1], &eptr, 10);
  wA = strtol(argv[2], &eptr, 10);
  hB = strtol(argv[3], &eptr, 10);
  wB = strtol(argv[4], &eptr, 10);

  MATRIX_TYPE *A = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));
  MATRIX_TYPE *B = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));

  if(wA != hB){
    printf("Nao é possivel multiplicar as duas matrizes!");
  }

  /* Initialize the two argument vectors */
  A->rows =  (float*)aligned_alloc(32, hA*wA*sizeof(float));
  B->rows = (float*)aligned_alloc(32, hB*wB*sizeof(float));
  float *result = (float*)aligned_alloc(32, hA*wB*sizeof(float));
  float *valueA_alligned = (float*) aligned_alloc(32, wB * sizeof(float));

  /* Initialize the three argument vectors */
  __m256 vec_rowsA = _mm256_set1_ps(5.0f);
  __m256 vec_rowsB = _mm256_set1_ps(2.0f);
__m256 vec_result = _mm256_set1_ps(0.0f);

  float *nxt_rowsA = A->rows; 
  float *nxt_rowsB = B->rows;
float *nxt_result = result;

float * start_rowsA = A->rows;
float* start_rowsB = B->rows;

  float* nxt_valueA = valueA_alligned;

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

  for(i=0;i<(hA * wB); i+=8, nxt_result+=8)
{
	_mm256_store_ps(nxt_result,vec_result);
}



  /* Compute the difference between the two vectors */
  nxt_rowsA = A->rows; 
  nxt_rowsB = B->rows; 
nxt_result = result;
nxt_valueA = valueA_alligned;

int aux = 0;
/*
for(int index = 0; index < (hA*wA);index++)
{
	printf("Matrix A: %.2f ---- %d\n", A->rows[index],index);
}

for(int index = 0; index < (hB*wB);index++)
{
	printf("Matrix B: %.2f ---- %d\n", B->rows[index],index);
}

for(int index = 0; index < (hA*wB);index++)
{
	printf("Matrix C: %.2f ---- %d\n", result[index],index);
}*/


  for( i = 0; i < (hA * wA); i += 8, nxt_rowsA += 8){

	printf("set1(%.2f) em vec_valueA\n", *nxt_rowsA);
    __m256 vec_valueA = _mm256_set1_ps(*nxt_rowsA);

		printf(".\n");
	for(unsigned int j = 0; j<wB; j+=1, nxt_rowsB+=8, nxt_result+=8)
	{
		printf("vec_valueB = load(nxt_rowsB), *nxt_rowsB = %.2f\n", *nxt_rowsB);
		__m256 vec_valueB = _mm256_load_ps(nxt_rowsB);
		printf("vec_result = load(nxt_result), *nxt_result = %.2f\n", *nxt_result);
		__m256 vec_result = _mm256_load_ps(nxt_result);

		printf("fmadd(%.2f,%.2f,%.2f)",*nxt_rowsA,*nxt_rowsB, *nxt_result);
		vec_result = _mm256_fmadd_ps(vec_valueA,vec_valueB,vec_result);
		_mm256_store_ps(nxt_result, vec_result);
		printf(" = %.2f\n", *nxt_result);
	
		printf("Stored(nxt_result). nxt_result = %.2f\n", *nxt_result);	
		printf(".\n");
	}
	if(aux < wA)
	{
		printf("Aux = %d menor q WA. nxt_result sai de: %.2f",aux, *nxt_result);
		nxt_result = nxt_result - (8*wB);
		printf(" para %.2f\n", *nxt_result);
		aux++;
		printf(".\n");		
	}
	else if(aux == wA)
	{
		printf("Aux = %d, igual a WA. nxt_rowsB sai de: %.2f", aux , *nxt_rowsB);
		nxt_rowsB = nxt_rowsB - (8*hB*wB);
		printf(" para %.2f\n", *nxt_rowsB);
		aux = 0;
		printf(".\n");
	}	

  }

  

  return 0;
}
