#include <iostream>
#include <fstream>
#include "Item.h"
using namespace std;

int main(){
	string filename = " ";
	int numItems =0;
	int capacity =0;
	//string names[numItems];
	//int wt[numItems];
	//int val[numItems];
	int tempWt =0;
	int tempVal =0;
	string tempName=" ";
	Item tempItem;
	
	cout << "\t 0-1 Knapsack Problem (DP Edition)" << endl;
	cout << "Enter the filename of your program: ";
	cin >> filename;
	
	ifstream inputFile(filename);
	
	if(!inputFile.is_open()){
		cout << "Error" << endl;
	}
	else{
		inputFile >> numItems >> capacity;
		
		Item items[numItems];
		
		for(int i=0; i<numItems; i++){
			inputFile >> tempName;
			inputFile >> tempVal;
			inputFile >> tempWt;
		
			tempItem = Item(tempVal, tempWt);
			tempItem.name = tempName;
			items[i] = tempItem;
			cout << items[i].name << " " << items[i].value << " " << items[i].weight << endl;
		}
		
		Item dp[numItems][capacity];
		
		for (int i = 0; i <= numItems; i++) {
			for (int w = 0; w <= capacity; w++) {
				if (i == 0 || w == 0)
					dp[i][w].value = 0; // Base case: no items or no capacity
				else if(items[w-1].weight <= w)
					dp[i][w].value = max(items[i - 1].value + dp[i - 1][w - 1].value, dp[i - 1][w].value);
				else
					dp[i][w].value = dp[i - 1][w].value;
			}
		}
		
		int totalVal = dp[numItems][capacity].value;
		
		cout << totalVal << endl;
	}
	
	return 0;
}