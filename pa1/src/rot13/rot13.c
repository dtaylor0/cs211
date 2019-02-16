#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	
	if(argc!=2) {
		printf("wrong # of args\n");
		return 0;
	}
	
	char* str1 = argv[1];
	int i;
	int current_letter;
	char changed_letter;
	for(i = 0; i < strlen(str1); i++) {
		current_letter = str1[i];
		if((current_letter >= 'a' && current_letter <= 'm') || (current_letter >= 'A' && current_letter <= 'M')) {
			changed_letter = current_letter + 13;
			printf("%c", changed_letter);
			continue;
		} else if((current_letter >= 'n' && current_letter <= 'z') || (current_letter >= 'N' && current_letter <= 'Z')) {
			changed_letter = current_letter - 13;
			printf("%c", changed_letter);
			continue;
		} else {
			changed_letter = current_letter;
			printf("%c", changed_letter);
			continue;
		}
	}
	printf("\n");
	return 0;
}

