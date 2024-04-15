/*CS351 Project 1 by Miles Mann and Anthony Signorelli*/
#include <iostream>
#include <fstream>
#include "Item.h"
#include "pq.h"
using namespace std;

int main(){
	ifstream filename;
	string tempName=" ";
	int tempValue=0;
	int tempWeight=0;
	int capacity=0;
	int numItems=0;
	int totalWt=0;
	int totalVal=0;
	Item tempItem;
	pq<Item> priority;
	int bagAmount = 0;
	string inputFile=" ";
	
	cout << "/****0-1 Knapsack Problem****/" << endl;
	cout << "What is the name of your txt file? ";
	cin >> inputFile;
	
	filename.open(inputFile);
	filename >> numItems >> capacity;
	
	//just reading in the file with an initial pq and making items for each
	for(int i=0; i<numItems; i++){
		filename >> tempName;
		filename >> tempValue;
		filename >> tempWeight;
		
		tempItem = Item(tempValue, tempWeight);
		tempItem.name = tempName;
		priority.enqueue(tempItem);
	}
	
	//getting info from OG pq and putting it in greedy alg pq
	pq<Item> knap;
	for(int i=0; i<numItems; i++){
		tempItem = priority.dequeue();
		if(totalWt + tempItem.weight <= capacity){
			knap.enqueue(tempItem);
			totalVal = totalVal + tempItem.value;
			totalWt = totalWt + tempItem.weight;
			bagAmount++;
		}
	}
	filename.close();
	
	ofstream output;
	string outputName=" ";
	cout << "Knapsack made! Enter the name of the file you want to output the results to: ";
	cin >> outputName;
	output.open(outputName);
	
	output << "Bag Amount: " << bagAmount << " Total Value: " << totalVal << " Total WT: " << totalWt << endl;
	output << "Items in the bags: " << endl;
	
	for(int i=0; i<bagAmount; i++){
		tempItem = knap.dequeue();
		output << tempItem.name << " " << tempItem.value << " " << tempItem.weight << endl;
	}
	output.close();
	
	cout << "goodbye." << endl;
	
	return 0;
}