#include "Item.h"
#include <iostream>
#include <fstream>
using namespace std;

int compareKnap(int capacity, int index, int value[], int weight[], int** doubleptr){
	//Base cases
	if(index < 0){
		return 0;
	}
	else if(doubleptr[index][capacity] != -1){
		cout << "compare base case" << endl;
		return doubleptr[index][capacity];
	}
	
	if(weight[index] > capacity){
		cout << "compare option 1" << endl;
		//store value in table and then continue recursion
		doubleptr[index][capacity] = compareKnap(capacity, index-1, value, weight, doubleptr);
		return doubleptr[index][capacity];
	}
	else{
		cout << "compare option 2" << endl;
		//store value in table and then continue recursion
		doubleptr[index][capacity] = max(value[index] + compareKnap(capacity-weight[index], index-1, value, weight, doubleptr), compareKnap(capacity, index-1, value, weight, doubleptr));
		return doubleptr[index][capacity];
	}
}

int dynamicKnap(int capacity, int numItems, int value[], int weight[]){
	int** doubleptr;
	doubleptr = new int*[numItems];
	
	for(int i = 0; i < numItems; i++){
		doubleptr[i] = new int[capacity+1];
		cout << "Part" << i << endl;
	}
	
	for(int i=0; i<numItems; i++){
		for(int j=0; j<(capacity+1); i++){
			
			doubleptr[i][j] = -1;
		}
		cout << "part" << i << endl;
	}
	
	return compareKnap(capacity, numItems-1, value, weight, doubleptr);
}

int main(){
	string inputFile(" ");
	int totalWt(0);
	int totalVal(0);
	ifstream filename;
	string tempName=" ";
	int tempValue(0);
	int tempWeight(0);
	int capacity(0);
	int numItems(0);
	int bagAmount(0);
	
	cout << "/****0-1 Knapsack Problem (Dyanmic Programming Edition)****/" << endl;
	cout << "What is the name of your txt file? ";
	cin >> inputFile;
	
	filename.open(inputFile);
	filename >> numItems >> capacity;
	
	int value[numItems];
	int weight[numItems];
	string names[numItems];
	
	//just reading in the file with an initial pq and making items for each
	for(int i=0; i<numItems; i++){
		filename >> tempName;
		filename >> tempValue;
		filename >> tempWeight;
		
		weight[i] = tempWeight;
		value[i] = tempValue;
		names[i] = tempName;
		
		cout << names[i] << " " << value[i] << " " << weight[i] << endl;
	}
	
	cout << dynamicKnap(capacity, numItems, value, weight) << endl;
	
	return 0;
}