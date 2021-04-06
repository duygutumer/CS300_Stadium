#include "Heap.h"
#include <iostream>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

////////////constructor////////////
Heap::Heap()
{
	heapElement zerothElement(0, 0, " ");
	seatNum.push_back(zerothElement);
}

////////////percolateDown////////////
void Heap::percolateDown(int idx)
{
	int Lchild; // left child (2i)
	int Rchild; // right child (2i + 1)
	int child; // general child (which one???) -> holds the child value

	int tmpSeat = seatNum[idx].seat;
	int tmpIdentity = seatNum[idx].identity;
	string tmpB = seatNum[idx].bName;

    for( ; idx < (seatNum.size()+1)/2; idx = child )
    {
        Lchild = idx * 2;
		Rchild = idx * 2 + 1;
		child = Lchild;
		if(Rchild < seatNum.size())
		{
			if(seatNum[Rchild].seat > seatNum[Lchild].seat || (seatNum[Rchild].seat == seatNum[Lchild].seat && seatNum[Rchild].identity < seatNum[Lchild].identity)) 
				child = Rchild;
		}
		// since I choose my child now I can look my tmp values
        if(seatNum[child].seat > tmpSeat || (seatNum[child].seat == tmpSeat && seatNum[child].identity < tmpIdentity ))
		{
			seatNum[idx] = seatNum[child];
			locations[seatNum[child].bName] = idx;
			
		}
        else
            break;
	}
	seatNum[idx].seat = tmpSeat;
	seatNum[idx].bName = tmpB;
	seatNum[idx].identity = tmpIdentity;
	locations[tmpB] = idx;
}

////////////insert////////////
void Heap::insert(const int idx, const int emptySeat, string bName)
{
	int hole = seatNum.size(); // it will be at the end
	heapElement element(0, 0, " ");
	seatNum.push_back(element); // but since I have to resize my vector with one more element 
	for( ; hole > 1 && (emptySeat > seatNum[hole/2].seat || (emptySeat == seatNum[hole/2].seat && idx < seatNum[hole/2].identity)); hole/= 2 )
	{
		seatNum[hole] = seatNum[hole / 2];
		locations[seatNum[hole/2].bName] = hole; // this block will be hole th index with bName of hole/2
	}
	seatNum[hole].seat = emptySeat;
	seatNum[hole].bName = bName;
	seatNum[hole].identity = idx;
	locations[bName] = hole;
}

////////////isEmpty////////////
bool Heap::isEmpty() const
{
	if(seatNum.size() <= 1)
	{
		return true;
	}
	return false;
}

////////////percolateUp////////////
void Heap::percolateUp(int idx)
{
	string tmpB = seatNum[idx].bName;
	int tmpId = seatNum[idx].identity;
	int tmpSeat = seatNum[idx].seat;

	for( ; idx > 1 && ((tmpSeat > seatNum[idx/2].seat) || (tmpSeat == seatNum[idx/2].seat && tmpId < seatNum[idx/2].identity)); idx = idx/2)
	{
		seatNum[idx] = seatNum[idx/2];
		locations[seatNum[idx/2].bName] = idx;
	}
	
	seatNum[idx].bName = tmpB;
	seatNum[idx].identity = tmpId;
	seatNum[idx].seat = tmpSeat;
	locations[tmpB] = idx;

}

////////////GetSeatNum////////////
int Heap::GetSeatNum()
{
	if(!isEmpty())
	{
		return seatNum[1].seat;
	}
	return -1;

}

////////////GetBlockName////////////
string Heap::GetBlockName()
{
	if(!isEmpty())
	{
		return seatNum[1].bName;
	}
	return " ";

}

////////////Remove////////////
void Heap::Remove()
{
	if(!isEmpty())
	{
		seatNum[1] = seatNum[seatNum.size()-1]; 
		locations[seatNum[seatNum.size()-1].bName] = 1;
		seatNum.pop_back();
		percolateDown(1);
	}

}

////////////decreaseEmptySeats////////////
void Heap::decreaseEmptySeats()
{   
	int newEmpty = seatNum[1].seat-1;
	int idy = seatNum[1].identity;
	string newName = seatNum[1].bName;
	Remove();
	insert(idy, newEmpty, newName);
	/*seatNum[1].seat--;
	percolateDown(1);*/

}

////////////decreaseEmptySeats////////////
void Heap::decreaseEmptySeats(string bn)
{   
	int idx = locations[bn];
	seatNum[idx].seat--;
	percolateDown(idx);

}

////////////increaseEmptySeats////////////
void Heap::increaseEmptySeats(string bn)
{
	int idx = locations[bn];
	seatNum[idx].seat++;
	percolateUp(idx);

}