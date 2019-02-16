#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main() {
	double *numbers;
	int number_count = 10;
	numbers = malloc(sizeof(double) * number_count);
	memset(numbers, 0, sizeof(double) * number_count);
	double mean = 0;
	int counter = 0;
	double temp;
	int i;
	for(i = 0; i < number_count; i++) {
		if(scanf(" %lf", &temp) != 1) {
			number_count = i;
			numbers = realloc(numbers, sizeof(double) * number_count);
			if(numbers == NULL) {
				return -1;
			}
			break;
		}
		numbers[i] = temp;
		if(i == number_count-1) {
			number_count = 2 * number_count;
			numbers = realloc(numbers, sizeof(double) * number_count);
			if(numbers == NULL) {
				return -1;
			}
		}
		mean += temp;
		counter++;
	}
	if(counter == 0) {
		printf("no data\n");
		return -1;
	}
	mean = mean / counter;
	double stddev = 0;
	for(i = 0; i < number_count; i++) {
		stddev += (numbers[i]-mean) * (numbers[i]-mean);
		
	}
	stddev = stddev / counter;
	stddev = sqrt(stddev);
	printf("mean: %.0f\n", mean);
	printf("stddev: %.0f\n", stddev);
	free(numbers);
	return 0;
}
