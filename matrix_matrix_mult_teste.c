#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include "timer.h"

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

  /* Initialize the three argument vectors */
  __m256 vec_rowsA = _mm256_set1_ps(5.0f);
  __m256 vec_rowsB = _mm256_set1_ps(2.0f);

  float *nxt_rowsA = A->rows; 
  float *nxt_rowsB = B->rows; 

  for (long unsigned int i = 0; 
	i < (hA * wB); 
	i += 8, nxt_rowsA += 8, nxt_rowsB += 8) {

	  /* Store the elements of the vectors in the arrays */
	  _mm256_store_ps(nxt_rowsA, vec_rowsA);
	  _mm256_store_ps(nxt_rowsB, vec_rowsB);
  }

  /* Compute the difference between the two vectors */
  nxt_rowsA = A->rows; 
  nxt_rowsB = B->rows;  
  float *nxt_result = result;
  int in = 0;

  /*for (i = 0; i < (hA * wB); i += 8, nxt_rowsA += 8) {
	  // Initialize the three argument vectors 
	  __m256 vec_rowsA = _mm256_load_ps(nxt_rowsA);

    for(ind = (i / hB); ind < hB; ind += 8, nxt_result += 8){
        __m256 vec_rowsB = _mm256_load_ps(nxt_rowsB);

        __m256 vec_half = _mm256_mul_ps(vec_rowsA, vec_rowsB);

        nxt_rowsB += 8 * hB;

        vec_rowsB = _mm256_load_ps(nxt_rowsB);

        __m256 vec_result = _mm256_fmadd_ps(vec_rowsA, vec_rowsB, vec_half);

        _mm256_store_ps(nxt_result, vec_result);

        if(inf == hB - 1){
          nxt_rowsB -= 8 * ((hA *wB) - ind));
        }

    }

  }*/

  cont = 0;

  for(i = 0 ; i < (hA * wB); i += 8, nxt_rowsA += 8, nxt_result += 8){ 

    //PROBLEMA AO TENTAR DAR RELOAD EM VEC_ROWSB
      printf("iteracao %d\n", iter );
      __m256 vec_rowsA = _mm256_load_ps(nxt_rowsA);
      __m256 vec_rowsB = _mm256_load_ps(nxt_rowsB);
      __m256 vec_half = _mm256_mul_ps(vec_rowsA, vec_rowsB);
      
      nxt_rowsB += (8 * wB);
      vec_rowsB = _mm256_load_ps(nxt_rowsB);

      __m256 vec_result = _mm256_fmadd_ps(vec_rowsA, vec_rowsB, vec_half);

      _mm256_store_ps(nxt_result, vec_result);

      if(iter % wA == 0){
          cont++;
          nxt_rowsB -= (8 * hB * wB) + 8 * cont;
        }
        iter++;
  }

  for(int index = 0 ; index < (hA*wB); index++){
    printf("%.2f\n", result[index]);
  }
  

  return 0;
}