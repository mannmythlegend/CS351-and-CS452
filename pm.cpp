#include <iostream>
#include <stdio.h>
#include <string.h>
#include <array>
#include <cmath>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 32 blah

void mergesort (int * a, int first, int last);
void smerge(int * a, int * b, int lasta, int lastb, int * output);
int Rank(int * a, int first, int last, int valToFind);
void pmerge(int * a, int * b, int lasta, int lastb, int * output = NULL);

int my_rank;
int p;

int main (int argc, char * argv[]) {
					
	int source;				/* rank of the sender */
	int dest;				/* rank of destination */
	int tag = 0;			// message number
	char message[100];		// message itself
	MPI_Status status;		// return status for receive
	
	// Start MPI
	MPI_Init(&argc, &argv);
	
	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	// THE REAL PROGRAM IS HERE//
    srand(69420);

    int n = 0;

	if(my_rank == 0)
	{
		cout << "Please enter the size of your array (Powers of 2 ONLY!): ";
		cin >> n;
	}

	//share n
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int * array = new int[n];
    bool used[501] = {false}; // for no repeats
	
	//fill array with random numbers
	if(my_rank == 0)
	{
    	//fill it with numbers 1-500
		for (int i=0;i<n;i++) {
        	int num = rand() % 500 + 1;

        	while(used[num])
            	num = rand() % 500 + 1;
			
        	used[num] = true;

       		array[i] = num;
    	}

		cout << "OG Array: " << endl;
		for(int i=0; i<n; i++)
			cout << array[i] << " ";
		cout << endl;
	}

	//share array
	MPI_Bcast(array, n, MPI_INT, 0, MPI_COMM_WORLD);

	mergesort(array, 0, n-1);

	if(my_rank == 0)
	{
		cout << "Sorted Array: " << endl;
		for(int i=0; i<n; i++)
		{
			cout << array[i] << " ";
		}
		cout << endl;
	}
    
    //memory-safe program
    delete [] array;

	// Shut down MPI
	MPI_Finalize();

	return 0;
}

void mergesort (int * a, int first, int last){
	if(first < last && last-first+1 >= 4)
	{
		int mid = ((first+last)/2);
		mergesort(a, first, mid);
		mergesort(a, mid+1, last);
		pmerge(&a[first], &a[mid+1], mid-first, last-mid-1); // pmerge
	}
	else
	{
		// this else means there are only two elements, and we need to swap
		if(a[last] < a[first])
		{
			int temp = a[last];
			a[last] = a[first];
			a[first] = temp;
		}
		return;
	}
}

void smerge(int * a, int * b, int lasta, int lastb, int * output){
	int size = lasta+lastb;
	int i=0, j=0, k=0;

	while(i<lasta && j<lastb)
	{
		if(a[i] < b[j]) {
			output[k] = a[i];
			k++; i++;
		}
		else
		{
			output[k] = b[j];
			k++; j++;
		}
	}

	while(i<lasta)
	{
		output[k] = a[i];
		k++; i++;
	}

	while(j<lastb)
	{
		output[k] = b[j];
		k++; j++;
	}
}

int Rank(int * a, int first, int last, int valToFind){
	while (first<=last)
    {
        int mid = (first+last) / 2;
        if(valToFind == a[mid])
            return mid; // a[mid] is it!
        else if (valToFind > a[mid]) 	
            first = mid+1; // recurse LHS
        else                        	
            last = mid-1; // recurse RHS
    }
    return first; // not found in array, so give original pos
}

void pmerge(int * a, int * b, int lasta, int lastb, int * output){
	//size calculation along with initializing locala and final (c) array
	int size = lasta + lastb +2;
	int c[size] = {0};
	int locala[size] = {0};
	
	//size for p(i)
	int numSamples = (size/2)/log2(size/2);

	//other arrays we need for rank and shape-making
	int aSelect[numSamples] = {0};
	int bSelect[numSamples] = {0};
	int localRA[numSamples] = {0};
	int localRB[numSamples] = {0};
	int sRankA[numSamples] = {0};
	int sRankB[numSamples] = {0};

	//find values for aSelect and bSelect
	for(int i=0;i<numSamples;i++)
	{
		int pos = i * log2(size/2);
		
		aSelect[i] = a[pos];
		bSelect[i] = b[pos];
	}
		
	//getting sRank vals in "pardo"
	for(int i=my_rank; i<numSamples; i+=p)
	{
		localRA[i] = Rank(b,0,size/2-1,aSelect[i]);
		localRB[i] = Rank(a,0,size/2-1,bSelect[i]);

		//put these values into the respective locala's
		int pos = i * log2(size/2);
		
		locala[localRA[i] + pos] = aSelect[i];
		locala[localRB[i] + pos] = bSelect[i];
	}

	//reduce local ranks all together for sRank
	MPI_Allreduce(&localRA, &sRankA, numSamples, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	MPI_Allreduce(&localRB, &sRankB, numSamples, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	
	// select array. starting the making of shapes
	int select[numSamples*2] = {0};
	
	if(my_rank == 0){
		
		//smerge partitions with overseer
		smerge(&aSelect[0], &bSelect[0],numSamples,numSamples,&select[0]);
		
	}

	// broadcast 0's select array to rest of world
	MPI_Bcast(&select, numSamples*2, MPI_INT, 0, MPI_COMM_WORLD);
	
	for(int i=my_rank; i<numSamples*2; i+=p)
	{
			int startA=0;
			int startB=0;

			//get start points in both arrays
			while(a[startA] <= select[i] && startA < size/2)
			{
				startA++;
			}

			while(b[startB] <= select[i] && startB < size/2)
			{
				startB++;
			}
			
			// we will incrememnt the ends like we did the starts later
			int endA = startA;
			int endB = startB;

			// goes into this if, if its the final shape
			if(i>= (numSamples*2)-1)
			{
				while(endA < size/2)
					endA++;
				while(endB < size/2)
					endB++;

				//smerge
				smerge(&a[startA], &b[startB], endA-startA, endB-startB, &locala[size-((endA-startA)+(endB-startB))]);
			}
			else
			{
				while(a[endA] < select[i+1] && endA < size/2)
					endA++;
				while(b[endB] < select[i+1] && endB < size/2)
					endB++;

				//smerge				
				smerge(&a[startA], &b[startB], endA-startA, endB-startB, &locala[startA+startB]);
			}
	}

	MPI_Allreduce(&locala, &c, size, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	// copy final array back to a, and were done!
	for(int i=0; i<size; i++)
		a[i] = c[i];
}