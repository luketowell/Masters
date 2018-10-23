#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char **argv)
{
	//declare all int values
	int length_of_array = 2000000;
	int i, root = 0;
	int myRank, numRanks;
	
	//declaration of clocks
	clock_t  start, end;
	
	//declare all doubles
	double *a, *b, *myA, *myB;
	double sum_of_a, sum_of_b, my_sum_of_a, my_sum_of_b,  mean_of_a, mean_of_b;
	double my_sum_of_sqrd_a_a_bar, my_sum_of_sqrd_b_b_bar , my_sum_of_a_times_b;
	double sum_of_sqrd_a_a_bar, sum_of_sqrd_b_b_bar, sum_of_a_times_b, correlationCoefficient, time_to_run;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &numRanks);
	
	if (myRank == root){
		//allocate all memory to the arrays
		a = (double *) malloc(length_of_array * sizeof(a));
		b = (double *) malloc(length_of_array * sizeof(b));

		//initialisation of the value arrays
		for (i = 0; i < length_of_array; i++){
			a[i] = sin(i);
			b[i] = sin(i+5);
		}
	}
	printf("number of ranks: %d \n", numRanks);
	
	if (length_of_array % numRanks != 0 ){
		printf("The size of the array is not evenly divisible amongst the nodes");
		}
	else {
		start = clock();
		//assign size for local arrays
		myA = (double *) malloc((length_of_array/numRanks) *sizeof(myA));
		myB = (double *) malloc((length_of_array/numRanks) *sizeof(myB));
		
		MPI_Scatter(&a[0], length_of_array/numRanks, MPI_DOUBLE, &myA[0], length_of_array/numRanks, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Scatter(&b[0], length_of_array/numRanks, MPI_DOUBLE, &myB[0], length_of_array/numRanks, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		for (i = 0; i < length_of_array/numRanks; i++){
			my_sum_of_a += myA[i];
			my_sum_of_b += myB[i];
		}
		
		//Gather all sums of a in and add them together.
		MPI_Reduce(&my_sum_of_a, &sum_of_a, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Reduce(&my_sum_of_b, &sum_of_b, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		
		//serial code
		mean_of_a = sum_of_a / length_of_array;
		mean_of_b = sum_of_b / length_of_array;
		
		//broadcast the mean for a and for b so that all nodes know whats going on
		MPI_Bcast(&mean_of_a,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
		MPI_Bcast(&mean_of_b,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
		
		//perform work out coefficients 
		for (i = 0; i < length_of_array/numRanks; i++){
			my_sum_of_sqrd_a_a_bar += (myA[i] - mean_of_a) * (myA[i] - mean_of_a);
			my_sum_of_sqrd_b_b_bar += (myB[i] - mean_of_b) * (myB[i] - mean_of_b);
			my_sum_of_a_times_b += (myA[i] - mean_of_a) * (myB[i] - mean_of_b);
		}
		
		MPI_Reduce(&my_sum_of_sqrd_a_a_bar, &sum_of_sqrd_a_a_bar, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Reduce(&my_sum_of_sqrd_b_b_bar, &sum_of_sqrd_b_b_bar, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Reduce(&my_sum_of_a_times_b, &sum_of_a_times_b, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		
		
		//serial code
		if(myRank == root){
			correlationCoefficient = sum_of_a_times_b / sqrt((sum_of_sqrd_a_a_bar * sum_of_sqrd_b_b_bar));
			end = clock();
			time_to_run = ((double)(end-start)) / CLOCKS_PER_SEC;
			printf("The time taken to execute the program = %.6f seconds \n", time_to_run);
			printf("The Pearson correlation coefficient is: %.4f \n", correlationCoefficient);
		}
	}
	MPI_Finalize();
}