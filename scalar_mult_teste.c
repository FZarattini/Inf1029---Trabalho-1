#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct matrix{
  unsigned long int height;
  unsigned long int width;
  float *rows;
}MATRIX_TYPE;

int main(int argc, char *argv[]) {
  long unsigned int h = 1<<21;
  long unsigned int w = 1<<21;
  float scalar;
  int index;
  char *eptr = NULL;

  // Convert arguments
  h = strtol(argv[1], &eptr, 10);
  w = strtol(argv[2], &eptr, 10);
  scalar = strtof(argv[3], NULL);

  MATRIX_TYPE *m = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));

  /* Initialize the two argument vectors */
  float *scalar_alligned =  (float*)aligned_alloc(32, h*w*sizeof(float));
  m->rows = (float*)aligned_alloc(32, h*w*sizeof(float));
  //float *c = (float*)aligned_alloc(32, N*sizeof(float));
  float *result = (float*)aligned_alloc(32, h*w*sizeof(float));

  /* Initialize the three argument vectors */
  __m256 vec_scalar = _mm256_set1_ps(scalar);
  __m256 vec_rows = _mm256_set1_ps(2.0f);

  float *nxt_scalar = scalar_alligned; 
  float *nxt_rows = m->rows; 

  for (long unsigned int i = 0; i < (h*w); i += 8, nxt_scalar += 8, nxt_rows += 8) {

	  /* Store the elements of the vectors in the arrays */
	  _mm256_store_ps(nxt_scalar, vec_scalar);
	  _mm256_store_ps(nxt_rows, vec_rows);
  }

  /* Compute the difference between the two vectors */
  nxt_scalar = scalar_alligned; 
  nxt_rows = m->rows; 
  float *nxt_result = result; 

  for (long unsigned int i = 0; i < h*w; i += 8, nxt_scalar += 8, nxt_rows += 8, nxt_result += 8) {
	  /* Initialize the three argument vectors */
	  __m256 vec_scalar = _mm256_load_ps(nxt_scalar);
	  __m256 vec_rows = _mm256_load_ps(nxt_rows);
	  //__m256 vec_c = _mm256_load_ps(nxt_c);

	  /* Compute the expression res = a * b + c between the three vectors */
	  __m256 vec_result = _mm256_mul_ps(vec_scalar, vec_rows);

	  /* Store the elements of the result vector */
	  _mm256_store_ps(nxt_result, vec_result);
  }

  m->rows = result;

  for(index = 0 ; index < (h*w); index++){
    printf("%.2f\n", m->rows[index]);
  }

  return 0;
}