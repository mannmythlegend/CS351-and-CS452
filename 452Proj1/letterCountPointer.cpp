#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <string.h>
#include <cstring>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 32 blah

int main (int argc, char * argv[]) {

	int my_rank;			// my CPU number for this process
	int p;					// number of CPUs that we have
	int source;				// my_rank of the sender
	int dest;				// my_rank of destination
	int tag = 0;			// message number
	char message[100];		// message itself
	MPI_Status status;		// return status for receive
	
	// Start MPI
	MPI_Init(&argc, &argv);
	
	// Find out my my_rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	// THE REAL PROGRAM IS HERE
	
	char * lines = nullptr;
	char * locala = nullptr;
	string filename = " ";
	string line =" ";
	int n=0;
	
	if (my_rank == 0) {
        // overseer reads the line from the external file
        cout << "What is the name of your txt: ";
		cin >> filename;
		
		ifstream input_file(filename);
        getline(input_file, line);

        // copy the line to char pointer w/ cstring
		n = line.length();
		lines = new char[n+1];
		strcpy(lines, line.c_str());

        input_file.close();

    }
	
	// broadcast n to all processes before scatter (for some reason if I didn't do this the scatter didn't work)
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	int remainder = n%p;
	int localn = n/p + remainder;
	
	locala = new char[localn+1];
	
	// scatter main pointer
	MPI_Scatter(&lines[0], localn, MPI_CHAR, &locala[0], localn, MPI_CHAR, 0, MPI_COMM_WORLD);
	
    // local work. count each time a letter pops up in local array
	locala[localn] ='\0';
    int * localCount = new int[26];
	fill(localCount, localCount + 26, 0);
	
	
    for (char*i=locala; *i!='\0'; *i++) {
        if (isalpha(*i)) {
            localCount[*i - 'a']++;
        }
    }

    // reduce the local counts to the global sum
    int * globalCount = new int[26];
    MPI_Reduce(&localCount[0], &globalCount[0], 26, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	// print each letter and their frequencies with static_cast
    if (my_rank == 0) {
        for (int i = 0; i < 26; i++) {
            cout << static_cast<char>('a' + i) << ": " << globalCount[i] << endl;
        }
		delete[] lines;
    }
	
	// valgrind valgrind valgrind ...
	delete [] locala;
	delete[] globalCount;
	delete[] localCount;
	
    MPI_Finalize();
    return 0;
}