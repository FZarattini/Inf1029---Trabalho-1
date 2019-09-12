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
    float *result = (float*)aligned_alloc(32, h*w*sizeof(float));

  	/* Initialize the three argument vectors */
  	__m256 vec_scalar = _mm256_set1_ps(scalar_value);
  	//__m256 vec_rows = _mm256_set1_ps(2.0f);

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

  if(wA != hB){
      printf("Dimensoes nao compativeis para multiplicacao!");
      return 0;
  }

  float *valueA_alligned = (float*) aligned_alloc(32, wB * sizeof(float));

  /* Initialize the three argument vectors */
  //__m256 vec_rowsA = _mm256_set1_ps(5.0f);
  //__m256 vec_rowsB = _mm256_set1_ps(2.0f);
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