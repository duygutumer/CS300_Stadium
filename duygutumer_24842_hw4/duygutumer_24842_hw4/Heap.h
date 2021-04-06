#ifndef _HEADER_H
#define _HEADER_H

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

// MAX HEAP IMPLEMENTATION
class Heap 
{

public:

	Heap(); //constructor
	//~Heap(); //destructor -> but not needed

	int GetSeatNum();
	string GetBlockName();
	bool isEmpty() const;
	
	void insert(const int idx, const int emptySeat, string bName); // percolate up in also in this function
	void percolateUp(int idx);
	void Remove(); // pop_up first most empty element(first one) and percolate down
	void decreaseEmptySeats(); // decrease the seat number and percolate down
	void decreaseEmptySeats(string bn); // decrease the seat number with given block name and percolate down
	void increaseEmptySeats(string bn); // increase the seat number with given block name and percolate up
	
private:

	struct heapElement
	{
		int seat; // num of empty seat
		string bName; // block name
		int identity; // holds the block name's identity since there is a priority

		heapElement::heapElement(int i, int v, string b)
		{
			identity = i;
			seat = v;
			bName = b;

		}
	};

	unordered_map<string, int> locations; // holds the location
	
	vector<heapElement> seatNum; // my heap vector 
	void percolateDown(int idx);
};
#endif

