#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

typedef struct matrix{
	unsigned long int height;
	unsigned long int width;
	float *rows;
}MATRIX_TYPE;

int scalar_matrix_mult(float scalar_value, struct matrix *matrix){

	return 0;

}

//argv -> Height, Width e escalar

int main(int argc, char* argv[]){
	int result;
	long unsigned int h = 1 << 21;
	long unsigned int w = 1 << 21;
	float scalar;
	char *eptr = NULL;

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

	result = scalar_matrix_mult(scalar, m);

	if(result == 1){
		printf("Erro ao multiplicar vetor!\n");
		return 1;
	}

	return 0;	
}