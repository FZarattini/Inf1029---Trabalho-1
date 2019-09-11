#include <stdio.h>
#include "matrix_lib.h"

int scalar_matrix_mult(float scalar_value, struct matrix *matrix){
	long unsigned int h = 1 << 21;
	long unsigned int w = 1 << 21;
	int index;
	h = matrix->height;
	w = matrix->width;

	/* Initialize the two argument vectors */
    float *scalar_alligned =  (float*)aligned_alloc(32, h*w*sizeof(float));
    matrix->rows = (float*)aligned_alloc(32, h*w*sizeof(float));
    float *result = (float*)aligned_alloc(32, h*w*sizeof(float));

  	/* Initialize the three argument vectors */
  	__m256 vec_scalar = _mm256_set1_ps(scalar_value);
  	__m256 vec_rows = _mm256_set1_ps(2.0f);

	  float *nxt_scalar = scalar_alligned; 
	  float *nxt_rows = matrix->rows; 

	  for (long unsigned int i = 0; i < (h*w); i += 8, nxt_scalar += 8, nxt_rows += 8) {

		  /* Store the elements of the vectors in the arrays */
		  _mm256_store_ps(nxt_scalar, vec_scalar);
		  _mm256_store_ps(nxt_rows, vec_rows);
	  }

	  /* Compute the difference between the two vectors */
	  nxt_scalar = scalar_alligned; 
	  nxt_rows = matrix->rows; 
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

	  matrix->rows = result;

	  for(index = 0 ; index < (h*w); index++){
	  	if(matrix->rows[index] != scalar_value*2.0f)
	  		return 0;
	    printf("%.2f ------- %d\n", matrix->rows[index], index);
	  }

	  return 1;
	}
