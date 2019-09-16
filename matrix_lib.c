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
    __m256 vec_rows;
  	__m256 vec_scalar = _mm256_set1_ps(scalar_value);

	  float *nxt_scalar = scalar_alligned; 
	  float *nxt_rows = matrix->rows; 

	  for (long unsigned int i = 0; i < (h*w); i += 8, nxt_scalar += 8, nxt_rows += 8) {

		  /* Store the elements of the vectors in the arrays */
		  _mm256_store_ps(nxt_scalar, vec_scalar);
	  }

	  /* Compute the difference between the two vectors */
	  nxt_scalar = scalar_alligned; 
	  nxt_rows = matrix->rows; 
	  float *nxt_result = result; 

	  for (long unsigned int i = 0; i < h*w; i += 8, nxt_scalar += 8, nxt_rows += 8, nxt_result += 8) {
		  /* Initialize the three argument vectors */
		  vec_scalar = _mm256_load_ps(nxt_scalar);
		  vec_rows = _mm256_load_ps(nxt_rows);

		  /* Compute the expression res = a * b + c between the three vectors */
		  __m256 vec_result = _mm256_mul_ps(vec_scalar, vec_rows);

		  /* Store the elements of the result vector */
		  _mm256_store_ps(nxt_result, vec_result);
	  }

	  matrix->rows = result;

	  nxt_rows = matrix->rows;
	
	for(int i = 0; i < h*w; i++)
	{
		if(nxt_rows == NULL)
		{
			return 0;
		}

		nxt_rows++;
	}

	  return 1;
	}


int matrix_matrix_mult(struct matrix *matrixA, struct matrix * matrixB, struct matrix * matrixC){
  long unsigned int hA = 1<<21;
  long unsigned int wA = 1<<21;
  long unsigned int hB = 1<<21;
  long unsigned int wB = 1<<21;
  long unsigned int i, ind, index;
  int bla;

  hA = matrixA->height;
  hB = matrixB->height;
  wA = matrixA->width;
  wB = matrixB->width;

  if(wA != hB){
      printf("Dimensoes nao compativeis para multiplicacao!");
      return 0;
  }

  /* Initialize the three argument vectors */
  __m256 vec_result = _mm256_set1_ps(0.0);	

  float* nxt_rowsA = (float*)aligned_alloc(32, hA*wA*sizeof(float)); 
  float* nxt_rowsB = (float*)aligned_alloc(32, hB*wB*sizeof(float));
  float* nxt_result = matrixC->rows;
  float* start_rowsA = matrixA->rows;
  float* start_rowsB = matrixB->rows;

  __m256 vec_rowsA;
  __m256 vec_rowsB;

  for (i = 0; 
	i < (hA * wA); 
	i += 8, nxt_rowsA += 8) {

	  /* Store the elements of the vectors in the arrays */
	  _mm256_store_ps(nxt_rowsA, vec_rowsA);
  }

  for(i = 0; i < (hB * wB); i += 8, nxt_rowsB += 8 ){
    _mm256_store_ps(nxt_rowsB, vec_rowsB);
  }

  for(i = 0; i < (hA * wB); i += 8, nxt_result += 8 ){
    _mm256_store_ps(nxt_result,vec_result);
  }

  //Printando vetores antes de calcular

  printf("-----------Vetores antes de calcular a multiplicacao de matrizes!-----------\n");
  for(bla = 0; bla != hA*wA; bla++){
  	printf("VETOR A: %.2f\n", matrixA->rows[bla]);
  }

  for(bla = 0; bla != hB*wB; bla++){
  	printf("VETOR B: %.2f\n", matrixB->rows[bla]);
  }

  for(bla = 0; bla != hA*wB; bla++){
  	printf("VETOR RESULTADO INICIALIZADO COM: %.2f\n", matrixC->rows[bla]);
  }


  /* Compute the difference between the two vectors */
  nxt_rowsA = matrixA->rows; 
  nxt_rowsB = matrixB->rows; 
  nxt_result = matrixC->rows;
  
  float *start_result = matrixC->rows;

  for( i = 0; i < (hA * wA); i += 1, nxt_rowsA += 1){

    __m256 vec_valueA = _mm256_set1_ps(*nxt_rowsA);

    if(i % wB == 0){
        nxt_rowsB = start_rowsB;
        nxt_result = start_result;
    }

    for(ind = 0 ; ind < wB; ind += 8, nxt_rowsB += 8){
      __m256 vec_rowsB = _mm256_load_ps(nxt_rowsB);

      for(index = 0 ; index < wB; index += 8, nxt_result += 8){
       
        __m256 vec_result = _mm256_load_ps(nxt_result);

        vec_result = _mm256_fmadd_ps(vec_valueA, vec_rowsB, vec_result);

       _mm256_store_ps(nxt_result, vec_result);

      }
    }  

  }

  nxt_result = matrixC->rows;
  for(int i = 0; i < hA*wB; i++)
  {
	if(nxt_result == NULL)
	{	
		return 0;
	}
	nxt_result++;
  }


  return 1;
}
