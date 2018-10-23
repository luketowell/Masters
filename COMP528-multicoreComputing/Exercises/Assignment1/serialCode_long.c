#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	//declare all int values
	int length_of_array = strtol(argv[1], NULL, 10);
	int i;
	
	//declare all doubles
	
	double a[length_of_array];
	double b[length_of_array];
	double a_minus_a_bar[length_of_array], b_minus_b_bar[length_of_array], sqrd_a_minus_bar[length_of_array], sqrd_b_minus_bar[length_of_array],a_times_b[length_of_array];
	double sum_of_a, sum_of_b, mean_of_a, mean_of_b, sum_of_sqrd_a_a_bar, sum_of_sqrd_b_b_bar , sum_of_a_times_b, correlationCoefficient ;
	
	//work out length of objects - No longer need this I dont think 
	size_t length_of_a = sizeof(a) / sizeof(double);
	size_t length_of_b = sizeof(b) / sizeof(double);
	
	//initialisation of the value arrays
	for (i = 0; i < length_of_array; i++){
		a[i] = sin(i);
		b[i] = sin(i+5);
	}
	
	//work out the mean
	//The if/else statement has been implemented incase functionality to provide your own data is introduced
	
	if (length_of_a == length_of_b){
		//foreach of the values add them all together - can be parallel
		for (i = 0; i < length_of_array; i++){
			sum_of_a += a[i];
			sum_of_b += b[i];
		}
	}
	else {  
		printf("The length of the provided arrays are not the same.\n Please check and try again.\n");
		return 0;
	}
	
	//work out means of the objects
	//serial code
	mean_of_a = sum_of_a / length_of_array;
	mean_of_b = sum_of_b / length_of_array;
	
	for (i = 0; i < length_of_array; i++){
		//a_minus_a_bar and b_minus_b_bar
		a_minus_a_bar[i] = a[i] - mean_of_a;
		b_minus_b_bar[i] = b[i] - mean_of_b;
		
		//find out sqrd x minux x bar results
		sqrd_a_minus_bar[i] = a_minus_a_bar[i] * a_minus_a_bar[i];
		sqrd_b_minus_bar[i] = b_minus_b_bar[i] * b_minus_b_bar[i];
		
		//a minus a bar multiplied by b minus b bar.
		a_times_b[i] = a_minus_a_bar[i] * b_minus_b_bar[i];
		
		//Sum of sqrd values
		sum_of_sqrd_a_a_bar += sqrd_a_minus_bar[i];
		sum_of_sqrd_b_b_bar += sqrd_b_minus_bar[i];
		sum_of_a_times_b += a_times_b[i];
	}
	
	//find out sqrd results
//	for (i = 0; i < length_of_a; i++){
//		sqrd_a_minus_bar[i] = a_minus_a_bar[i] * a_minus_a_bar[i];
//		sqrd_b_minus_bar[i] = b_minus_b_bar[i] * b_minus_b_bar[i];
//	}
//	
//	//a times b array
//	for(i = 0; i < length_of_a; i++){
//		a_times_b[i] = a_minus_a_bar[i] * b_minus_b_bar[i];
//	}
//	
//	//sum of values
//	for (i = 0; i < length_of_a; i++){
//		sum_of_sqrd_a_a_bar += sqrd_a_minus_bar[i];
//		sum_of_sqrd_b_b_bar += sqrd_b_minus_bar[i];
//		sum_of_a_times_b += a_times_b[i];
//	}
	//final print line to put input on the last line.
	
	//serial code
	correlationCoefficient = sum_of_a_times_b / sqrt((sum_of_sqrd_a_a_bar * sum_of_sqrd_b_b_bar));
	printf("The Pearson correlation coefficient is: %.3f \n", correlationCoefficient);
	
	return 0;
}