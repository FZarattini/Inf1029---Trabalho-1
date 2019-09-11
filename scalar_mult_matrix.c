#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <string.h>

typedef struct matrix{
  unsigned long int height;
  unsigned long int width;
  float *rows;
}MATRIX_TYPE;

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

	int main(int argc, char *argv[]) {
	  long unsigned int h = 1<<21;
	  long unsigned int w = 1<<21;
	  float scalar;
	  char *eptr = NULL;
	  int resultado;

	  // Check arguments
	  if (argc != 4) {
	        printf("Usage: %s <vector_length>\n", argv[0]);
	        return 0;
	  }

	  // Convert arguments
	  h = strtol(argv[1], &eptr, 10);
	  w = strtol(argv[2], &eptr, 10);
	  scalar = strtof(argv[3], NULL);

	  if (h == 0 || w == 0 || scalar == 0) {
        printf("%s: erro na conversao do argumento: errno = %d\n", argv[0], errno);

        /* If a conversion error occurred, display a message and exit */
        if (errno == EINVAL)
        {
            printf("Conversion error occurred: %d\n", errno);
            return 1;
        }

        /* If the value provided was out of range, display a warning message */
        if (errno == ERANGE) {
            printf("The value provided was out of rangei: %d\n", errno);
            return 1;
		}
   	 }

	  MATRIX_TYPE *m = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));
	  m->height = h;
	  m->width = w;

	  resultado = scalar_matrix_mult(scalar, m);

	  if (resultado == 1){
	  	printf("Sucesso!\n");
	  }else{
	  	printf("Erro de calculo!\n");
	  }

	  return 0;
}