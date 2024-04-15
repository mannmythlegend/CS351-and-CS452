//CS351 Project by Miles Mann and Anthony Signorelli
#include <iostream>
#include <vector>
#include "Item.h"
#include <fstream>

using namespace std;

//backtracking function
void knapsackBacktracking(int capacity, int currentIndex, int currentWeight, int currentValue, vector<Item>& items, vector<Item>& tempKnap, vector<Item>& knap, int& bestValue) {
    //base case - if we have gone through all items
    if (currentIndex == items.size()) {
        // Update the best solution if the current one is better
        if (currentValue > bestValue) {
            bestValue = currentValue;
            knap = tempKnap;
        }
        return;
    }

    //include item if it won't exceed cap (promising check)
    if (currentWeight + items[currentIndex].weight <= capacity) {
        tempKnap.push_back(items[currentIndex]);
        knapsackBacktracking(capacity, currentIndex + 1, currentWeight + items[currentIndex].weight,
                             currentValue + items[currentIndex].value, items, tempKnap, knap, bestValue);
        tempKnap.pop_back(); // Backtracking after we've fully searched that node's options to find a possible better option
    }

    //exclude item
    knapsackBacktracking(capacity, currentIndex + 1, currentWeight, currentValue, items, tempKnap, knap, bestValue);
}

int main() {
    vector<Item> items;
    int capacity = 0;
	string filename = " ";
	ifstream inputFile;
    vector<Item> tempKnap;
    vector<Item> knap;
    int bestValue = 0;
	int numItems =0;
	string tempName = " ";
	int tempVal =0;
	int tempWt =0;
	Item tempItem;
	
	cout << "\tKnapsack 0-1 Problem (Backtracking Edition!)" << endl;
	cout << "Enter the name of the txt: ";
	cin >> filename;
	inputFile.open(filename);
	inputFile >> numItems >> capacity;
	
	for(int i=0; i<numItems; i++){
		inputFile >> tempName >> tempVal >> tempWt;
		tempItem = Item(tempVal, tempWt);
		tempItem.name = tempName;
		items.push_back(tempItem);
	}

    knapsackBacktracking(capacity, 0, 0, 0, items, tempKnap, knap, bestValue);

    //print results
	cout << "Capacity: " << capacity << endl;
	cout << "Total Items in Knap: " << knap.size() << endl;
    cout << "Items in the Knapsack:" << endl;
    for (int i=0; i<knap.size(); i++) {
        cout << "\t[" << i+1 << "] " << knap[i].name << " Value - " << knap[i].value << " Weight - " << knap[i].weight << endl;
    }

    cout << "Max value: " << bestValue << endl;
	
	inputFile.close();
	
	cout << "Where would you like to export results? ";
	cin >> filename;
	
	ofstream outputFile;
	outputFile.open(filename, ios::app);
	outputFile << "Total Items in Knapsack: " << knap.size() << endl;
	outputFile << "Capacity: " << capacity << "\nItems in Knapsack" << endl;
	
	for (int j=0; j<knap.size(); j++) {
        outputFile << "\t[" << j+1 << "] " << knap[j].name << " Value - " << knap[j].value << " Weight - " << knap[j].weight << endl;
    }
	outputFile << "Max Value: " << bestValue << endl;
	
	outputFile.close();
	cout << "Goodbye." << endl;
	
    return 0;
}
