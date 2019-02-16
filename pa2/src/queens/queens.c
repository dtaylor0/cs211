#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int counter = 0;
int Q_count = 0;
int hasPrinted = 0;
typedef struct Queen {
	int height;
	int width;
} Queen;

typedef struct TempQ {
	int height;
	int width;
} TempQ;

Queen *Qs;
TempQ *Ts;

void print(char **);
int qIsValid(char **, int, int);
int valid();
void countQs(char **); 
void storeQueens(char **, int); 
char** newBoard(char **);
void addQueens(char **, int);

int main(int argc, char *argv[]) {
	char **board = (char**)malloc(sizeof(char*)*8);
	Ts = (TempQ*)malloc(sizeof(TempQ)*3);
	int i;
	for(i = 0; i < 3; i++) {
		Ts[i].height = -1;
	}
	for(i = 0; i < 8; i++) {
		board[i] = (char*)malloc(sizeof(char)*8);
	}
	
	if(argc == 2) {
		FILE *fp = fopen(argv[1], "r");
		int j;
		for(i = 0; i < 8; i++) {
			for(j = 0; j < 8; j++) {
				char temp;
				fscanf(fp, " %c", &temp);
				board[i][j] = temp;
			}
		}
		fclose(fp);
		countQs(board);
		storeQueens(board, counter);
		int isValid = valid(board);
		if(!isValid) {
			free(Qs);
			free(board);
			return 0;
		}
		char **new_board = newBoard(board);
		print(new_board);
		free(new_board);
		free(board);
		free(Qs);
		free(Ts);
	} else if(argc == 3) {
		FILE *fp = fopen(argv[2], "r");
		int j;
		for(i = 0; i < 8; i++) {
			for(j = 0; j < 8; j++) {
				char temp;
				fscanf(fp, " %c", &temp);
				board[i][j] = temp;
			}
		}
		fclose(fp);
		countQs(board);
		storeQueens(board, counter);
		int isValid = valid(board);
		if(!isValid) {
			free(Qs);
			free(board);
			return 0;
		}
		char **new_board = newBoard(board);
		char* command = argv[1];
		if(strcmp("+2", command) == 0) {
			print(new_board);
			for(i = 0; i < 8; i++) {
				for(j = 0; j < 8; j++) {
					if(new_board[i][j] == 'q') {
						addQueens(new_board, j);
						return 0;
					}
				}
			}
			printf("Zero\n");
			return 0;
		} else if(strcmp("-w", command) == 0) {
			printf("Invalid\n");
		}
		free(new_board);
		free(board);
		free(Qs);
		free(Ts);
	} else if(argc == 4) {
		printf("Invalid\n");
	}
	return 0;
}

void print(char **board) {
	int i;
	int j;
	for(i = 0; i < 8; i++) {
		for(j = 0; j < 8; j++) {
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
	
}

void countQs(char **board) {
	
	int i;
	int j;
	for(i = 0; i < 8; i++) {
		for(j = 0; j < 8; j++) {
			if(board[i][j] == 'Q') {
				counter++;
			}
		}
	}
}

void  storeQueens(char **board, int countQs) {
	Qs = (Queen*)malloc(sizeof(Queen)*countQs);
	int i;
	int j;
	int k=0;
	for(i = 0; i < 8; i++) {
                for(j = 0; j < 8; j++) {
                        if(board[i][j] == 'Q') {
                                Qs[k].height = i;
				Qs[k].width = j;
				k++;
                        }
                }
        }
	return;
}

int valid() {
	int i, j, h_diff, w_diff;
	for(i = 0; i < counter; i++) {
		for(j = i+1; j < counter; j++) {
			if(Qs[i].height == Qs[j].height || Qs[i].width == Qs[j].width) {
				printf("Invalid\n");
				return 0;
			}
			h_diff = Qs[i].height - Qs[j].height;
			w_diff = Qs[i].width - Qs[j].width;
			if(h_diff < 0) {
				h_diff = h_diff * -1;
			}
			if(w_diff < 0) {
				w_diff = w_diff * -1;
			}
			if(h_diff == w_diff) {
				printf("Invalid\n");
				return 0;
			}
		}
	}
	return 1;
}

char** newBoard(char** board) {
	char ** new = (char**)malloc(sizeof(char*)*8);
	int i;
	for(i = 0; i < 8; i++) {
		new[i] = (char*)malloc(sizeof(char)*8);
	}
	int j, k, h_diff, w_diff;
	for(i = 0; i < 8; i++) {
		for(j = 0; j < 8; j++) {
			new[i][j] = 'q';
		}
	}
	for(i = 0; i < counter; i++) {
		for(j = 0; j < 8; j++) {
			new[Qs[i].height][j] = '.';
		}
		for(j = 0; j < 8; j++) {
			new[j][Qs[i].width] = '.';
		}
		for(j = 0; j < 8; j++) {
			for(k = 0; k < 8; k++) {
				h_diff = Qs[i].height - j;
				w_diff = Qs[i].width - k;
				if(h_diff < 0) {
					h_diff = h_diff * -1;
				}
				if(w_diff < 0) {
					w_diff = w_diff * -1;
				}
				if(h_diff == w_diff) {
					new[j][k] = '.';
				}
			}
		}
	}
	for(i = 0; i < counter; i++) {	
		new[Qs[i].height][Qs[i].width] = 'Q';
	}
	return new;
}

void addQueens(char **board, int column) {
	if(column > 7) {
		if(hasPrinted == 0) {
			printf("One\n");
			hasPrinted = 1;
		}
		return;
	}
	if(Q_count >= 2) {
		if(hasPrinted == 0) {
			printf("Two or more\n");
			hasPrinted = 1;
		}
		return;
	}
	int i;
	int j = 0;
	for(i = 0; i < 8; i++) {
		if(board[i][column] == 'q' && qIsValid(board, i, column)) {
			Q_count++;
			while(Ts[j].height != -1) {
				j++;
			}
			Ts[j].height = i;
			Ts[j].width = column;
			column++;
			addQueens(board, column);
		}
	}
	column++;
	addQueens(board, column);
	
}

int qIsValid(char **board, int h, int w) {
	int i, h_diff, w_diff;
	for(i = 0; i < counter; i++) {
		if(Qs[i].height == h && Qs[i].width == w) {
			return 0;
		}
		h_diff = Qs[i].height - h;
		w_diff = Qs[i].width - w;
		if(h_diff < 0) {
			h_diff = h_diff * -1;
		}
		if(w_diff < 0) {
			w_diff = w_diff * -1;
		}
		if(h_diff == w_diff) {
			return 0;
		}
	}
	for(i = 0; i < Q_count; i++) {
		if(Ts[i].height == h && Ts[i].width == w) {
			return 0;
		}
		h_diff = Ts[i].height - h;
		w_diff = Ts[i].width - w;
		if(h_diff < 0) {
			h_diff = h_diff * -1;
		}
		if(w_diff < 0) {
			w_diff = w_diff * -1;
		}
		if(h_diff == w_diff) {
			return 0;
		}
	}
	return 1;
}
