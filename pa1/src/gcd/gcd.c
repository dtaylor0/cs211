#include <stdio.h>
#include <stdlib.h>

void gcd(int input_a, int input_b) {
	int temp_switch;
	if (input_a % input_b == 0) {
		printf("%d\n",input_b);
		return;
	} else {
		temp_switch = input_b;
		input_b = input_a % input_b;
		input_a = temp_switch;
		gcd(input_a, input_b);
		
	}
	return;
}

int main(int argc, char **argv) {
	int a,b;
	
	if (argc != 3){
		printf("Error: Number of arguments is wrong.\n");
		return  0;
	}
	
	
	a = atoi(argv[1]);
	b = atoi(argv[2]);
	int temp;
	if (a == 0) {
		printf("%d\n", b);
		return 0;
	} else if (b == 0) {
		printf("%d\n", a);
		return 0;
	}
	if (b > a) {
		temp = a;
		a = b;
		b = temp;
	}
	gcd(a,b);
	
	return 0;
}
