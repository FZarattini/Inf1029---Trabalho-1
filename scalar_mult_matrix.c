#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

typedef struct matrix{
	unsigned long int height;
	unsigned long int width;
	float *rows;
}MATRIX_TYPE;

int scalar_matrix_mult(float scalar_value, struct matrix *matrix){
	long unsigned int h = matrix->height;
	long unsigned int w = matrix->width;
	long unsigned int i;

	float* scalar_alligned = (float*)aligned_alloc(32, h*w*sizeof(float)); 

	__m256 scalar_vec = _mm256_load_ps(scalar_alligned);
	scalar_vec = _mm256_set1_ps(scalar_value);

	__m256 rows_vec = _mm256_load_ps(matrix->rows);

	float *nxt_scalar = scalar_alligned;
	float *nxt_rows = matrix->rows;

	printf("altura %lu\n", h);
	printf("largura %lu\n", w);
	printf("escalar %.2f\n", scalar_value);

	for(i = 0; i < (h*w); i += 8, nxt_rows += 8, nxt_scalar += 8){
		//printf("primeiro for %lu\n", i);
		_mm256_store_ps(nxt_scalar, scalar_vec);
		_mm256_store_ps(nxt_rows, rows_vec);
	}

	nxt_rows = matrix->rows;
	nxt_scalar = scalar_alligned;

	for(i = 0; i < (h*w); i += 8, nxt_scalar += 8, nxt_rows += 8){
		//printf("iteracao %d\n", i);
		//printf("segundo for %lu\n", i);
		__m256 rows_vec = _mm256_load_ps(nxt_rows);
		__m256 scalar_vec = _mm256_load_ps(nxt_scalar);
		
		rows_vec = _mm256_mul_ps(scalar_vec, rows_vec);

		_mm256_store_ps(nxt_rows, rows_vec);
		//_mm256_store_ps(nxt_scalar, scalar_vec);
	}


	for(i = 0; i < h*w; i++){
		printf("%.1f \n", matrix->rows[i]);
	}
		
	free(scalar_alligned);
	return 0;

}

//argv -> Height, Width e escalar

int main(int argc, char* argv[]){
	int i;
	int result;
	long unsigned int h = 1 << 21;
	long unsigned int w = 1 << 21;
	float scalar;
	char *eptr = NULL;
	float* rows_aux;

	if(argc != 4){
		printf("Quantidade de parâmetros inesperada! %d \n", argc);
		return 1;
	}

	h = strtol(argv[1], &eptr, 10);
	w = strtol(argv[2], &eptr, 10);
	scalar = strtof(argv[3], NULL);

	if(h == 0 || w == 0){
		printf("Argumento 0!\n");
		return 1;
	}

	MATRIX_TYPE *m = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));

	m->height = h;
	m->width = w;
	m->rows = (float*)aligned_alloc(32, h*w*sizeof(float));

	if(m->rows == NULL){
		printf("Erro ao alocar linhas da matriz!\n");
	}

	__m256 row_values = _mm256_load_ps(m->rows);

	row_values = _mm256_set1_ps(5.0f);

	m->rows = (float*)&row_values;	

	result = scalar_matrix_mult(scalar, m);

	if(result == 1){
		printf("Erro ao multiplicar vetor!\n");
		return 1;
	}else{
		printf("sucesso\n");
	}

	free(m);
	return 0;	
}