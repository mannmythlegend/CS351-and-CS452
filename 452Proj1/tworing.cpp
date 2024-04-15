#include <iostream>
#include <stdio.h>
#include <string.h>
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
	if(p%2 == 0){
		if(my_rank % 2 == 0){
			sprintf(firstRing, "First Ring: ");
			if(my_rank == 0){
				sprintf(firstRing+strlen(firstRing), "%d", my_rank);
				MPI_Send(firstRing, strlen(firstRing)+1, MPI_CHAR, my_rank+2, tag, MPI_COMM_WORLD);
				MPI_Recv(firstRing, 100, MPI_CHAR, p-2, tag, MPI_COMM_WORLD, &status);
				sprintf(firstRing+strlen(firstRing), "%d", my_rank);
				cout << firstRing << endl;
			}
			else{
				MPI_Recv(firstRing, 100, MPI_CHAR, my_rank-2, tag, MPI_COMM_WORLD, &status);
				sprintf(firstRing+strlen(firstRing), "-->%d-->", my_rank);
				MPI_Send(firstRing, strlen(firstRing)+1, MPI_CHAR, (my_rank+2)%p, tag, MPI_COMM_WORLD);
			}
		}
		else{
			sprintf(secondRing, "Second Ring: ");
			if(my_rank == 1){
				sprintf(secondRing+strlen(secondRing), "%d", my_rank);
				MPI_Send(secondRing, strlen(secondRing)+1, MPI_CHAR, p-1, tag, MPI_COMM_WORLD);
				MPI_Recv(secondRing, 100, MPI_CHAR, my_rank+2, tag, MPI_COMM_WORLD, &status);
				sprintf(secondRing+strlen(secondRing), "%d", my_rank);
				cout << secondRing << endl;
			}
			else{
				MPI_Recv(secondRing, 100, MPI_CHAR, (my_rank+2)%p, tag, MPI_COMM_WORLD, &status);
				sprintf(secondRing+strlen(secondRing), "-->%d-->", my_rank);
				MPI_Send(secondRing, strlen(secondRing)+1, MPI_CHAR, my_rank-2, tag, MPI_COMM_WORLD);
			}
		}
	}
	else{
		if(my_rank % 2 == 0){
			sprintf(firstRing, "First Ring: ");
			if(my_rank == 0){
				sprintf(firstRing+strlen(firstRing), "%d", my_rank);
				MPI_Send(firstRing, strlen(firstRing)+1, MPI_CHAR, my_rank+2, tag, MPI_COMM_WORLD);
				MPI_Recv(firstRing, 100, MPI_CHAR, p-1, tag, MPI_COMM_WORLD, &status);
				sprintf(firstRing+strlen(firstRing), "%d", my_rank);
				cout << firstRing << endl;
			}
			else{
				MPI_Recv(firstRing, 100, MPI_CHAR, my_rank-2, tag, MPI_COMM_WORLD, &status);
				sprintf(firstRing+strlen(firstRing), "-->%d-->", my_rank);
				if(my_rank == p-1){
					MPI_Send(firstRing, strlen(firstRing)+1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
				}
				else{
					MPI_Send(firstRing, strlen(firstRing)+1, MPI_CHAR, my_rank+2, tag, MPI_COMM_WORLD);
				}
			}
		}
		else{
			sprintf(secondRing, "Second Ring: ");
			if(my_rank == 1){
				sprintf(secondRing+strlen(secondRing), "%d", my_rank);
				MPI_Send(secondRing, strlen(secondRing)+1, MPI_CHAR, p-2, tag, MPI_COMM_WORLD);
				MPI_Recv(secondRing, 100, MPI_CHAR, my_rank+2, tag, MPI_COMM_WORLD, &status);
				sprintf(secondRing+strlen(secondRing), "%d", my_rank);
				cout << secondRing << endl;
			}
			else{
				if(my_rank == p-2){
					MPI_Recv(secondRing, 100, MPI_CHAR, 1, tag, MPI_COMM_WORLD, &status);
				}
				else{
					MPI_Recv(secondRing, 100, MPI_CHAR, my_rank+2, tag, MPI_COMM_WORLD, &status);
				}
				sprintf(secondRing+strlen(secondRing), "-->%d-->", my_rank);
				MPI_Send(secondRing, strlen(secondRing)+1, MPI_CHAR, my_rank-2, tag, MPI_COMM_WORLD);
			}
		}
	}

	// Shut down MPI
	MPI_Finalize();

	return 0;
}