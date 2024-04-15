/*CS351 Project 1 by Miles Mann and Anthony Signorelli*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// "struct" is just like "class" except that everything is public by default
// You need to define the four functions listed below to use this function.
// You will not be able to use a priority queue without defining the operator function.

struct Item {
	//Item variables
	string name;
	double value;
	double weight;
	
	//using this like a copy constructor
	Item (double, double);
	
	//Item functions
	double ratio() const;
	void print() const;
	bool operator<(const Item &) const;
	
	//default constructor (rybar things)
	Item(){};
};

Item::Item(double val, double wt){
	value = val;
	weight = wt;
}

double Item::ratio() const{
	return value / weight;
}

// Compares one item's ratio to another
bool Item::operator<(const Item & right) const {
    // Fill this in here!
	return ratio() < right.ratio();
}

void Item::print() const{
	cout << name << ": Value - " << value << " Weight - " << weight << endl;
}