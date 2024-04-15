//Random list generator by Anthony Signorelli and Miles Mann
#include <iostream>
#include <fstream>
using namespace std;

int main(){
	int numItems=0;
	int capacity=0;
	string filename=" ";
	int tempVal = 0;
	int tempWt = 0;
	
	cout << "Hello! How many items do you need? ";
	cin >> numItems;
	cout << "What about the capacity? ";
	cin >> capacity;
	cout << "Ok, what should your file name be?(please put .txt at the end of name) ";
	cin >> filename;
	
	ofstream output;
	output.open(filename);
	output << numItems << " " << capacity << endl;
	
	//r1 gives 5n for value to be randomized with and r2 gives 3n for weight.
	int r1=5*numItems;
	int r2=3*numItems;
	
	for(int i=0; i<numItems; i++){
		tempVal = rand() % r1 + 1;
		tempWt = rand() % r2 + 1;
		
		output << (i+1) << " " << tempVal << " " << tempWt << endl;
	}
	
	output.close();
	
	cout << "All done!" << endl;
	
	return 0;
}