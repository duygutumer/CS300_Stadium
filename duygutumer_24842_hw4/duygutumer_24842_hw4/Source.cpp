#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "Heap.h"

using namespace std;

/////////////////////GLOBAL VERIABLES/////////////////////
unordered_map <string, unordered_map<string, vector<string>>> stadium; // for holding all stadium
unordered_map <string, vector<string>> customerName; // for holding names
unordered_map <string, Heap> holdsHeap; // holds my heap values with given row name (as key)
vector<string> blockName, rowName; 
ofstream myfile;
int colNum;

/////////////////////print/////////////////////
/*Complexity of this function is O(B*R*C)*/
void print()
{
	myfile << endl;
	for(int i = 0; i < blockName.size(); i++)
	{
		myfile << blockName[i] << endl;
		myfile << "~~~~~~~" << endl;

		for(int j = 0; j < rowName.size(); j++)
		{
			myfile << rowName[j] << " : ";

			for(int k = 0; k < colNum; k++)
			{
				myfile << stadium[blockName[i]][rowName[j]][k] << " ";
			}
			myfile << endl;
		}
		myfile << "=======" << endl;
		myfile << endl;
	}
}

/////////////////////reserve_seat_by_row/////////////////////
/*Complexity of this function is O(logB + C)*/
void reserve_seat_by_row(string name, string rowNameString)
{
	bool checkEmptyness = false;

	int seat = holdsHeap[rowNameString].GetSeatNum();
	string blockName = holdsHeap[rowNameString].GetBlockName();
	string name3 = name.substr(0, 3);

	if (seat != -1  && customerName.find(name)== customerName.end()) // means that if heap is not empty and there is no reservation with the given name
	{ 
		// decrease the seat number and insert it new version 
		holdsHeap[rowNameString].decreaseEmptySeats(); // this takes O(logB) time
		
		int totalSeat = stadium[blockName][rowNameString].size();
		int col;

		for(col = 0; col < totalSeat; col++ && !checkEmptyness) // this takes O(C) time
		{
			if(stadium[blockName][rowNameString][col] == "---")
			{
				// change the stadium with given name
				stadium[blockName][rowNameString][col] = name3; // change the name

				// put the customer in to customerName
				customerName[name].push_back(blockName); // block name
				customerName[name].push_back(rowNameString); // row name
				customerName[name].push_back(to_string(col)); // column num

				checkEmptyness = true;
				break;
			}
		}
		if(checkEmptyness)
		{
			myfile << name << " has reserved " << blockName << " " <<  rowNameString << "-" << col << " by emptiest block" << endl;
		}
	}
	if(!checkEmptyness)
	{
		myfile << name << " could not reserve a seat!" << endl;
	}
}

/////////////////////reserve_seat/////////////////////
/*Complexity of this function is O(logB)*/
void reserve_seat(string uname, string blockNameString, int columnNumber, string rowNameString)
{
	string name3 = uname.substr(0, 3);

	// I have to check if reservation is empty and there isn't any reservation with the given name 
	if(stadium[blockNameString][rowNameString][columnNumber] == "---" && customerName.find(uname)== customerName.end()) 
	{
		// decrease the seat number and insert it new version
		holdsHeap[rowNameString].decreaseEmptySeats(blockNameString); // this takes O(logB) time

		// change the stadium with given name
		stadium[blockNameString][rowNameString][columnNumber] = name3; 

		// put the customer in to customerName
		customerName[uname].push_back(blockNameString); // block name
		customerName[uname].push_back(rowNameString); // row name
		customerName[uname].push_back(to_string(columnNumber)); // column num

		myfile << uname << " has reserved " << blockNameString << " " <<  rowNameString << "-" << columnNumber << endl;
	}
	else
	{
		myfile << uname << " could not reserve a seat!" << endl;
	}
}

/////////////////////get_seat/////////////////////
/*Complexity of this function is O(1)*/
void get_seat(string name)
{
	if(customerName.find(name) == customerName.end()) // if cannot find the name 
	{
		myfile << "There is no reservation made for " << name << "!" << endl;
	}
	else // if there is a reservation with given name
	{
		myfile << "Found that " << name  << " has a reservation in " << customerName[name][0] << " "<< customerName[name][1] << "-" << customerName[name][2] << endl; // Konya-A1-5
	}
}

/////////////////////cancel_reservation/////////////////////
/*Complexity of this function is O(logB)*/
void cancel_reservation(string cname)
{
	if(customerName.find(cname) == customerName.end()) // if cannot find the name 
	{
		myfile << "Could not cancel the reservation for " << cname << "; no reservation found!" << endl;
	}
	else // if finds the name 
	{
		string bn = customerName[cname][0]; // block name
		string rn = customerName[cname][1]; // row name
		string cn = customerName[cname][2]; // col num
		
		// delete the reservation with the given name
		customerName.erase(cname); 
		
		// increase the seat number and insert it new version
		holdsHeap[rn].increaseEmptySeats(bn); // this takes O(logB) time

		// change the stadium and make it empty again
        stadium[bn][rn][stoi(cn)] = "---"; 

		myfile << "Cancelled the reservation of " <<  cname << endl; // Konya-A1-5

	}
}

int main()
{
	ifstream text;

	string line, blockNameString, rowNameString, operation;
	int blockNum = 0;

	text.open("input.txt");
	myfile.open("output.txt");
	
	int count = 1; // this variable is a helper which shows the num of the line 

	// this while loop will works for first three lines
	while(count < 4 && getline(text, line)) 
	{
		istringstream input_file(line); 

		if (count == 1) // if it is the first line it should take the block name
		{
			while (input_file >> blockNameString)
			{
				blockNum++; // get the number of blocks since you want to create a Heap
				blockName.push_back(blockNameString);
			}
		}
		else if(count == 2) // if it is the second line it should take the rows name
		{
			while (input_file >> rowNameString)
			{
				rowName.push_back(rowNameString);
			}
		}
		else if(count == 3) // if it is the third line it should take the column number
		{
			input_file >> colNum;
		}
		count++; // must correct the situation 
	}
	
	/////////////////////STADIUM/////////////////////
	for(int i = 0; i < blockName.size(); i++)
	{
		for(int j = 0; j < rowName.size(); j++)
		{
			for(int k = 0; k < colNum; k++)
			{
				stadium[blockName[i]][rowName[j]].push_back("---");
			}
			
		}
	}
	
	/////////////////////holdsHeap/////////////////////
	for(int j = 0; j < rowName.size(); j++) // in here I implement my heap function according to their row names
	{   
		Heap myHeap = Heap();
		for(int i = 0; i < blockName.size(); i++)
		{
			myHeap.insert(i+1, colNum, blockName[i]);
		}
		holdsHeap[rowName[j]] = myHeap;
	}

	// now operations will start after first three lines
	while(getline(text, line))
	{
		istringstream input_file(line); 

		input_file >> operation; // which operation (print or reserve_seat or get_seat or... )

		if(operation == "print")
		{
			print();
		}
		else if(operation == "reserve_seat_by_row")
		{
			string sname;

			input_file >> sname;
			input_file >> rowNameString;
			
			reserve_seat_by_row(sname, rowNameString);

		}
		else if(operation == "reserve_seat")
		{
			string uname;
			int columnNumber;

			input_file >> uname;
			input_file >> blockNameString;
			input_file >> rowNameString;
			input_file >> columnNumber;

			reserve_seat(uname, blockNameString, columnNumber, rowNameString);
		}
		else if(operation == "get_seat")
		{
			string name;

			input_file >> name;
			get_seat(name);
		}
		else if(operation == "cancel_reservation")
		{
			string cname;

			input_file >> cname;
			cancel_reservation(cname);
		}
		else // for safety but actually I do not need it
		{
			myfile << ""; 
		}
	}

	text.close();
	myfile.close();

	return 0;
}
