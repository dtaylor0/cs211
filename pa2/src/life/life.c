#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int r_length;
int c_length;

void print(char**, int, int);
int countNeighbors(char **, int, int);
char** step(char**, int, int);

int main(int argc, char *argv[]) {
	int steps = atoi(argv[1]);
	if(steps < 0) {
		return EXIT_FAILURE;
	}
	FILE *fp;
	fp = fopen(argv[2], "r");
	fscanf(fp, " %d %d", &r_length, &c_length);
	char **board = (char**)malloc(sizeof(char*)*r_length);
	int i, j;
	for(i = 0; i < c_length; i++) {
		board[i] = (char*)malloc(sizeof(char)*c_length);
	}
	char current;
	for(i = 0; i < r_length; i++) {
		for(j = 0; j < c_length; j++) {
			if(fscanf(fp, " %c", &current) != 1) {
				return -1;
			}
			board[i][j] = current;
		}
	}
	fclose(fp);
	for(i = 0; i < steps; i++) {
		board = step(board, r_length, c_length);
	}
	print(board, r_length, c_length);
	free(board);
	return 0;
}

void print(char **board, int h, int w) {
	int i, j;
	for(i = 0; i < h; i++) {
		for(j = 0; j < w; j++) {
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
	return;
}


char** step(char** board, int h, int w) {
	int i, j, neighbors;
	char **res = (char**)malloc(sizeof(char*)*h);
	for(i = 0; i < h; i++) {
		res[i] = (char*)malloc(sizeof(char)*w);
	}
	for(i = 0; i < h; i++) {
		for(j = 0; j < w; j++) {
			neighbors = countNeighbors(board, i, j);
			if(board[i][j] == '*') {
				if(neighbors < 2 || neighbors > 3) {
					res[i][j] = '.';
				} else {
					res[i][j] = '*';
				}
			} else {
				if(neighbors == 3) {
					res[i][j] = '*';
				} else {
					res[i][j] = '.';
				}
			}
		}
	}
	return res;
}

int countNeighbors(char **board, int row, int col) {
	int i, j, r, c;
	int counter = 0;
	for(i = row - 1; i < row + 2; i++) {
		for(j = col - 1; j < col + 2; j++) {
			if(i == row && j == col) {
				continue;
			}
			c = j;
			r = i;
			if(j < 0)
				c = c_length - 1;
			else if(j >= c_length) {
				c = 0;
			}
			if(i < 0) {
				r = r_length - 1;
			} else if(i >= r_length) {
				r = 0;
			}
			if(board[r][c] == '*') {
				counter++;
			}
		}
	}
	return counter;
}
