#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

typedef struct matrix{
	unsigned long int height;
	unsigned long int width;
	float *rows;
}MATRIX_TYPE;



////////////////////////////////////////////
int matrix_matrix_mult(struct matrix *matrixA, struct matrix * matrixB, struct matrix * matrixC)
{
	printf("Ola estamos na funcao matrix x matrix\n");
	long unsigned int h = matrixA->height;
	long unsigned int w = matrixA->width;
	long unsigned int i;

	/*long unsigned int h2 = matrixB->height;
	long unsigned int w2 = matrixB->width;

	long unsigned int h3 = matrixC->height;
	long unsigned int w3 = matrixC->width;*/

	__m256 rows_vecA = _mm256_load_ps(matrixA->rows);
	__m256 rows_vecB = _mm256_load_ps(matrixB->rows);
	__m256 rows_vecC = _mm256_load_ps(matrixC->rows);

	float *nxt_rowsA = matrixA->rows;
	float *nxt_rowsB = matrixB->rows;
	float *nxt_rowsC = matrixC->rows; 

	for(i = 0; i < h*w; i++){
		printf("Pre passar pelo store_ps	matrixA: %.1f 	matrixB: %.1f	matrixC: %.1f  \n", matrixA->rows[i],matrixB->rows[i], matrixC->rows[i]);
	}

	printf("Passando pelo store_ps\n\n\n");
	for(i = 0; i < (h*w); i += 8, nxt_rowsA += 8, nxt_rowsB += 8){
		_mm256_store_ps(nxt_rowsA, rows_vecA);
		_mm256_store_ps(nxt_rowsB, rows_vecB);
		_mm256_store_ps(nxt_rowsC, rows_vecC); 
	}


	for(i = 0; i < h*w; i++){
		printf("Pos passar pelo store_ps	matrixA: %.1f 	matrixB: %.1f	matrixC: %.1f  \n", matrixA->rows[i],matrixB->rows[i], matrixC->rows[i]);
	}


	nxt_rowsA = matrixA->rows;
	nxt_rowsB = matrixB->rows;
	nxt_rowsC = matrixC->rows; 
	float *nxt_result_matrixC = matrixC->rows;

	for(i = 0; i < (h*w); i += 8, nxt_rowsA += 8, nxt_rowsB += 8, nxt_result_matrixC+=8)
	{
		__m256 rows_vecA = _mm256_load_ps(nxt_rowsA);
		__m256 rows_vecB = _mm256_load_ps(nxt_rowsB);
		
		__m256 vec_result_matrixC = _mm256_mul_ps(rows_vecA, rows_vecB);

		_mm256_store_ps(nxt_result_matrixC, vec_result_matrixC);
	}


	for(i = 0; i < h*w; i++){
		printf("Resultado da matrix x matrix: %.1f \n", matrixC->rows[i]);
	}
		

	return 1;
}

//argv -> Height, Width

int main(int argc, char* argv[]){
	int i;
	int result;
	long unsigned int h = 1 << 21;
	long unsigned int w = 1 << 21;
	char *eptr = NULL;
	float* rows_aux;

	if(argc != 3){
		printf("Quantidade de parâmetros inesperada! %d \n", argc);
		return 1;
	}

	h = strtol(argv[1], &eptr, 10);
	w = strtol(argv[2], &eptr, 10);

	if(h == 0 || w == 0){
		printf("Argumento 0!\n");
		return 1;
	}

	MATRIX_TYPE *mA = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));
	MATRIX_TYPE *mB = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));
	MATRIX_TYPE *mC = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE));

	mA->height = h;
	mA->width = w;
	mA->rows = (float*)aligned_alloc(32, h*w*sizeof(float));

	mB->height = h;
	mB->width = w;
	mB->rows = (float*)aligned_alloc(32, h*w*sizeof(float));

	mC->height = h;
	mC->width = w;
	mC->rows = (float*)aligned_alloc(32, h*w*sizeof(float));

	if(mA->rows == NULL){
		printf("A Erro ao alocar linhas da matriz!\n");
	}

	else if(mB->rows == NULL){
		printf("B Erro ao alocar linhas da matriz!\n");
	}


	else if(mC->rows == NULL){
		printf("C Erro ao alocar linhas da matriz!\n");
	}


	__m256 row_valuesA = _mm256_load_ps(mA->rows);
	row_valuesA = _mm256_set1_ps(5.0f);
	mA->rows = (float*)&row_valuesA;


	for(i = 0; i < h*w; i++){
		printf("ma %.1f \n", mA->rows[i]);
	}
		
	
	/////////////////////////////////////

	__m256 row_valuesB = _mm256_load_ps(mB->rows);
	row_valuesB = _mm256_set1_ps(2.0f);
	mB->rows = (float*)&row_valuesB;	



	for(i = 0; i < h*w; i++){
		printf("Mb: %.1f \n", mB->rows[i]);
	}
		

	/////////////////////////////////////

	__m256 row_valuesC = _mm256_load_ps(mC->rows);
	row_valuesC = _mm256_set1_ps(0.0f);
	mC->rows = (float*)&row_valuesC;	


	for(i = 0; i < h*w; i++){
		printf("mc %.1f \n", mC->rows[i]);
	}
		

	result = matrix_matrix_mult(mA, mB, mC);

	if(result == 0){
		printf("Erro ao multiplicar vetor!\n");
		return 1;
	}else{
		printf("sucesso\n");
	}

	free(mA);
	free(mB);
	free(mC);

	return 0;	
}
