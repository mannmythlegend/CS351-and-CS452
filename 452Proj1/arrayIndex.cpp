#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 32 blah

int main (int argc, char * argv[]) {

	int my_rank;			// my CPU number for this process
	int p;					// number of CPUs that we have
	int source;				// rank of the sender
	int dest;				// rank of destination
	int tag = 0;	// message number
	char message[100]; // message itself
	char * firstRing = message;
	char * secondRing = message;
	MPI_Status status;		// return status for receive
	
	
	// Start MPI
	MPI_Init(&argc, &argv);
	
	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	// THE REAL PROGRAM IS HERE
	
	srand(1241);
	int n = 300000;
	int localn = n / p;
	int *array = nullptr;
	
	//Let process 0 make the array and fill with random numbers from 1-50
	if(my_rank == 0){
		array = new int[n];
		for(int i=0; i<n; i++){
			array[i] = rand() % 50;
		}
	}
	
	//scattering the array
	int *locala = new int[localn];
	MPI_Scatter(array, localn, MPI_INT, locala, localn, MPI_INT, 0, MPI_COMM_WORLD);
	
	//finding local maxes, mins, and sums
	long int localMin = locala[0];
	long int localMax = locala[0];
	double localSum = locala[0];
	for(int i=1; i<localn; i++){
		localMax = max((int)localMax, locala[i]);
		localMin = min((int)localMin, locala[i]);
		localSum += locala[i];
	}
	
	//use mpi reduce to reduce local results to global results
	long int globalMax =0;
	long int globalMin =0;
	double globalSum =0.0;
	MPI_Reduce(&localMax, &globalMax, 1, MPI_LONG, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&localMin, &globalMin, 1, MPI_LONG, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&localSum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	//calculate average
	double average = globalSum / n;
	
	//we will print results on process 0
	if(my_rank == 0){
		cout << "Max is " << globalMax << endl;
		cout << "Min is " << globalMin << endl;
		cout << "Average is " << average << endl;	
		
		//might want to destruct a pointer of size one million.. #sorrythomas
		delete[] array;
	}
	
	//and destruct all the localas
	delete[] locala;
	
	MPI_Finalize();
	
	return 0;
}