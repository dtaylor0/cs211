#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	if(argc!=2) {
		printf("wrong # of args\n");
		return 0;
	}

	char* str1 = argv[1];
	int str1_len = strlen(str1);
	char *str2 = (char *)malloc(sizeof(char)*(2 * str1_len + 1));
	str2[0] = '\0';
	int counter = 0;
	
	char next_char;
	char current_char;
	int i;
	for(i = 0; i < strlen(str1); i++) {
		current_char = str1[i];
		if(current_char >= '0' && current_char <= '9') {
                        printf("error\n");
                        return 0;
                }
		counter++;
		char count_convert[10];
		char temp_str[10] = "";
		if(i == strlen(str1)-1) {
			temp_str[0] = current_char;
                        snprintf(count_convert, 10, "%d", counter);
                        strcat(str2, temp_str);
			strcat(str2, count_convert);
			counter = 0;
			break; 
		}
		next_char = str1[i+1];
		if(next_char == current_char) {
			continue;
		} else {
			temp_str[0] = current_char;
			snprintf(count_convert, 10, "%d", counter);
			strcat(temp_str, count_convert);
                        strcat(str2, temp_str);
			counter = 0; 
		}
	}
	if(str1_len >= strlen(str2)) {
		printf("%s\n", str2);
	} else {
		printf("%s\n", str1);
	}
	free(str2);
	return 0;
}
