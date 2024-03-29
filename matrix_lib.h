#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct matrix {
	unsigned long int height;
	unsigned long int width;
	float *rows;
}MATRIX_TYPE ;

int scalar_matrix_mult(float scalar_value, struct matrix *matrix);
int matrix_matrix_mult(struct matrix *matrixA, struct matrix * matrixB, struct matrix * matrixC);