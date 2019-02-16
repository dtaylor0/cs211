#include <stdio.h>
#include <stdlib.h>

int** multiply(int **, int **, int, int);

void print(int **, int);

int main(int argc, char *argv[]) {
	FILE *fp;
	fp = fopen(argv[1], "r");
	if(fp == NULL) {
		return -1;
	}
	int m_size;
	if(fscanf(fp, " %d", &m_size) != 1) {
		return -1;
	}
	int **matrix = (int**)malloc(sizeof(int*)*m_size);
	int **result = (int**)malloc(sizeof(int*)*m_size);
	int i;
	int j;
	for(i = 0; i < m_size; i++) {
		matrix[i] = (int*)malloc(sizeof(int) * m_size);
	}
	for(i = 0; i < m_size; i++) {
		result[i] = (int*)malloc(sizeof(int) * m_size);
	}
	for(i = 0; i < m_size; i++) {
		for(j = 0; j < m_size; j++) {
			if(fscanf(fp, " %d", &matrix[i][j]) != 1) {
				return -1;
			}
		}
	}
	
	int power;
	if(fscanf(fp, " %d", &power) != 1) {
                return -1;
        }
	result = multiply(matrix, result, m_size, power);
	print(result, m_size);
	
	for(i = 0; i < m_size; i++) {
		free(matrix[i]);
	}
	for(i = 0; i < m_size; i++) {
		free(result[i]);
	}
	free(matrix);
	free(result);
	return 0;
}

int** multiply(int **m, int **r, int size, int power) {
	int temp = 0;
	int i, j, k, a;
	int **temp_m = (int**)malloc(sizeof(int*)*size);
	
	if(power == 0) {
		for(i = 0; i < size; i++) {
			for(j = 0; j < size; j++) {
				if(i == j) {
					r[i][j] = 1;
				} else {
					r[i][j] = 0;
				}
			}
		}
		return r;
	}
	for(i = 0; i < size; i++) {
		temp_m[i] = (int*)malloc(sizeof(int) * size);
	}
	for(i = 0; i < size; i++) {
		for(j = 0; j < size; j++) {
			r[i][j] = m[i][j];
		}
	}
	//controls number of multiplications
	for(i = 1; i < power; i++) {
		//controls location in r[][]
		for(j = 0; j < size; j++) {
			for(k = 0; k < size; k++) {
				//creates r value to store
				for(a = 0; a < size; a++) {
					temp += r[j][a] * m[a][k];
				}
				temp_m[j][k] = temp;
				temp = 0;
			}
		}
		for(j = 0; j < size; j++) {
			for(k = 0; k < size; k++) {
				r[j][k] = temp_m[j][k];
			}
		}
	}
	for(i = 0; i < size; i++) {
		free(temp_m[i]);
	}
	free(temp_m);
	return r;
}

void print(int **m, int size) {
	int i;
	int j;
	for(i = 0; i < size; i++) {
                for(j = 0; j < size; j++) {
                        printf("%d", m[i][j]);
			if(j < size-1) {
				printf(" ");
			}
                }
                printf("\n");
        }
	return;
}
