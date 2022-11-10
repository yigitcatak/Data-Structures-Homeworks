#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
//My priority queue template class i had from last homework, modified it to binary min heap instead of priority queue
#include "MPQ.h"
#include "MPQ.cpp"
using namespace std;

#define INPUT "inputs.txt"
#define OUTPUT "outputs.txt"

// to track the reservation of a customer, to sen to unorderd_map
struct full_reservation
{
	string block;
	string row;
	int col;

	full_reservation(): block(""),row(""),col(-1){};
	full_reservation(string b, string r, int c)
	{
		block = b;
		row = r;
		col = c;
	}
};

// space complexity is a bit high due to time complexity constraints we have
vector<string> Block_names; // for block names during print, a name can be accesed in O(1) by index of that block
vector<string> Row_names; // for row names during print, a name can be accesed in O(1) by index of that row

unordered_map<string,int> Block_indexes; // for getting the index of a block given its name in O(1)
unordered_map<string,int> Row_indexes; // for getting the index of a row given its name in O(1)
unordered_map<string,full_reservation> Reservations; // for getting the full reservation information given the customer name
int R=0,B=0,C=0; // for storing the number of Rows, Blocks and Columns

void reserve_seat(string ***stadium, const string &name, const full_reservation &reserve, vector<unordered_map<string,row_info>> &Row_fullness, vector<MPQ<row_info>>&Row_queue)
{
	ofstream outf;
	outf.open(OUTPUT, ios::app);

	if( Reservations.find(name) == Reservations.end()) // if person does not have a reservation
	{
		if( stadium[Block_indexes[reserve.block]][Row_indexes[reserve.row]][reserve.col] != "" ) // if seat is full
		{
			outf << name << " could not reserve a seat!" << endl;
			outf.close();
			return;
		}


		// set a reservation
		Reservations[name] = reserve; 

		// Blocks and Rows are unordered maps therefore pulling the block and row index from there is done in const time
		stadium[Block_indexes[reserve.block]][Row_indexes[reserve.row]][reserve.col] = name.substr(0,3);

		// Adjust the Row queue for next reserve by row operation
		int index, row;
		index = Row_fullness[Block_indexes[reserve.block]][reserve.row].index; // get index to be removed from binary heap
		row = Row_indexes[reserve.row]; 
		Row_fullness[Block_indexes[reserve.block]][reserve.row].full_seat++; // update row info
		// since these are directly reached from hashes of unordered map const time

		

		Row_queue[row].Remove(index,Row_fullness,Row_names); // remove outdated row info;
		Row_queue[row].Insert(Row_fullness[Block_indexes[reserve.block]][reserve.row],Row_fullness,Row_names); // add updated row info 
		// since it is a binary heap, removing and adding is done in log(b) time, O(2logB) approximates O(logB)

		// output
		outf << name << " has reserved " << reserve.block << " " << reserve.row << "-" << reserve.col << endl;
	}
	else
		outf << name << " could not reserve a seat!" << endl;
	
	outf.close();
}

void reserve_seat_by_row(string ***stadium, const string &name, const string &row, vector<unordered_map<string,row_info>> &Row_fullness,vector<MPQ<row_info>> &Row_queue)
{
	ofstream outf;
	outf.open(OUTPUT, ios::app);

	if( Reservations.find(name) == Reservations.end()) // if person does not have a reservation
	{
		// get the block
		row_info temp;
		Row_queue[Row_indexes[row]].getMin(temp,Row_fullness,Row_names); // get the min from the queue to temp O(1), this pops min, shifts other entries so O(log(B)) time

		// get the column
		int col;
		for( int c = 0; c<C; c++ ) // among all columns (from left to right), O(C)
		{
			if( stadium[temp.block][temp.row][c] == "" ) // if empty column found
			{
				col = c;
				stadium[temp.block][temp.row][c] = name.substr(0,3); // update the stadium for print too
				break;
			}
			if( c == C-1 ) // if no proper column found
			{
				outf << name << " could not reserve a seat!" << endl; // output
				Row_queue[Row_indexes[row]].Insert(Row_fullness[temp.block][Row_names[temp.row]],Row_fullness,Row_names); // insert the row info back to row queue
				outf.close();
				return;
			}
		}

		Row_fullness[temp.block][Row_names[temp.row]].full_seat++; // as proper seat is now found, update row info
		Row_queue[Row_indexes[row]].Insert(Row_fullness[temp.block][Row_names[temp.row]],Row_fullness,Row_names); // add updated row info, again just an insert to heap O(log(B))

		// make the reservation 
		full_reservation temp2(Block_names[temp.block],Row_names[temp.row],col);
		Reservations[name] = temp2;

		// so overall we have 2*O(log(B)) + O(C), approximating O(log(B)+C)

		// output
		outf << name << " has reserved " << temp2.block << " " << temp2.row << "-" << temp2.col << " by emptiest block" << endl;
	}
	else
		outf << name << " could not reserve a seat!" << endl;
	
	outf.close();
}

void reserve_cancel(string ***stadium, const string &name, vector<unordered_map<string,row_info>> &Row_fullness, vector<MPQ<row_info>> &Row_queue)
{
	ofstream outf;
	outf.open(OUTPUT, ios::app);

	if( Reservations.find(name) == Reservations.end()) // if customer does not have a reservation
		outf << "Could not cancel the reservation for " << name << "; no reservation found!\n";

	else
	{
		// uninitialise

		// Adjust the Row queue for next reserve by row operation
		int index,row;
		index = Row_fullness[Block_indexes[Reservations[name].block]][Reservations[name].row].index; // get index to be removed from binary heap
		row = Row_indexes[Reservations[name].row];
		Row_fullness[Block_indexes[Reservations[name].block]][Reservations[name].row].full_seat--; // update row info 
		// since these are directly reached from hashes of unordered map const time

		Row_queue[row].Remove(index,Row_fullness,Row_names); // remove outdated row info;
		Row_queue[row].Insert(Row_fullness[Block_indexes[Reservations[name].block]][Reservations[name].row],Row_fullness,Row_names); // add updated row info 
		// since it is a binary heap, removing and adding is done in log(b) time, O(2logB) approximates O(logB)

		// reaching the reservation data with key:"name" and index of block/row with key:"block/row name" is const time
		stadium[Block_indexes[Reservations[name].block]][Row_indexes[Reservations[name].row]][Reservations[name].col] = "";
		Reservations.erase(name); 

		// output
		outf << "Cancelled the reservation of " << name << endl;
	}
	outf.close();		
}

void reserve_get(const string &name)
{
	ofstream outf;
	outf.open(OUTPUT, ios::app);

	if( Reservations.find(name) == Reservations.end()) // if person does not have a reservation
		outf << "There is no reservation made for " << name << "!\n";
	
	else
		outf << "Found that " << name << " has a reservation in " << Reservations[name].block 
		<< " " << Reservations[name].row << "-" << Reservations[name].col << endl;
		
	outf.close();
}

void Print(string ***stadium)
{
	ofstream outf;
	outf.open(OUTPUT, ios::app);

	// iterate over each Block + each Row + each Column
	for( int b=0; b<B; b++ )
	{
		outf << endl << Block_names[b] << endl << "~~~~~~~";
		for( int r=0; r<R; r++)
		{
			outf << endl << Row_names[r] << " : ";
			for ( int c=0; c<C; c++ )
			{
				if( stadium[b][r][c] != "" )
					outf << stadium[b][r][c] << " ";
				else
					outf << "--- ";
			}
		}
		outf << endl << "=======" << endl;
	}
	outf << endl;
	outf.close();
}

void DoTheThing()
{
	ifstream file;
	file.open(INPUT);

	enum state{readBLOCKS,readROWS,readCOL,readINPUTS};
	enum state2{NAME,BLOCK,ROW,COL};
	enum method{UNINIT,bySEAT,byROW,CANCEL,GET};

	string line,subString,block,name,row;
	int col;

	state S = readBLOCKS;
	state2 S2 = NAME;
	method M = UNINIT;

	// first three line of input - stadium properties
	for(int i=0; i<3; i++)
	{
		getline(file,line);
		istringstream stream(line);
		while(stream>>subString)
		{ 
			switch(S)
			{
				case readBLOCKS:
					Block_indexes[subString] = B;
					Block_names.push_back(subString);
					B++;
					break;
				case readROWS:
					Row_indexes[subString] = R;
					Row_names.push_back(subString);
					R++;
					break;
				case readCOL:
					C = stoi(subString);
					break;
			}
		}
		switch(S)
		{
			case readBLOCKS:
				S = readROWS;
				break;
			case readROWS:
				S = readCOL;
				break;
			case readCOL:
				S = readINPUTS;
				break;
		}
	}

	// initialise a stadium matrix with dimension [Block][Row][Column]
	string ***stadium = new string**[B];
	for(int i=0;i<B;i++)
	{
		stadium[i] = new string*[R];
		for(int j=0;j<R;j++)
			stadium[i][j] = new string[C];
	}

	// initialise min heap for selection of row in reserve by row, and unordered map to find given row's index in heap in const time

	vector<unordered_map<string,row_info>> Row_fullness(B);
	vector<MPQ<row_info>> Row_queue(R);
	for( int r=0; r<R; r++ )
	{
		cout << endl << r << endl << endl;
		MPQ<row_info> temp(B);
		for( int b=0; b<B; b++ )
		{
			row_info temp2(b,r,0);
			Row_fullness[b][Row_names[r]] = temp2;
			temp.Insert(temp2,Row_fullness,Row_names);
		}
		Row_queue[r] = temp;
	}

	// rest of the file - inputs
	while(getline(file,line))
	{
		if(line == "print")
			Print(stadium);

		else // this if else coul have been inside the next while loop but, it would initialise the line to istringstream
		// and read from it unnecessarily each time line is print
		{
			istringstream stream(line);
			while(stream>>subString)
			{ 
				switch(M)
				{
					// reserve method is uninitialised yet
					case UNINIT:
						if( subString == "reserve_seat" )
							M = bySEAT;
						else if( subString == "reserve_seat_by_row" )
							M = byROW;
						else if( subString == "cancel_reservation" )
							M = CANCEL;
						else
							M = GET;
						break;

					// reserve method is reserve by seat
					case bySEAT:
						switch(S2)
						{
							case NAME:
								name = subString;
								S2 = BLOCK;
								break;
							case BLOCK:
								block = subString;
								S2 = ROW;
								break;
							case ROW:
								row = subString;
								S2 = COL;
								break;

							case COL:
								col = stoi(subString);
								full_reservation reserve(block,row,col);
								reserve_seat(stadium, name, reserve, Row_fullness, Row_queue);

								// re-init these as reading is complete for row
								S2 = NAME;
								M = UNINIT;
								break;
						}
						break;

					// reserve method is reserve by row
					case byROW:
						switch(S2)
						{
							case NAME:
								name = subString;
								S2 = ROW;
								break;
							case ROW:
								reserve_seat_by_row(stadium, name, subString, Row_fullness, Row_queue);

								// re-init
								S2 = NAME;
								M = UNINIT;
						}
						break;

					// reserve method is cancel
					case CANCEL:
						reserve_cancel(stadium, subString, Row_fullness, Row_queue);

						//re-init
						M = UNINIT;
						break;

					// reserve method is get seat
					case GET:
						reserve_get(subString);

						//re-init
						M = UNINIT;
						break;
				}
			}
		}
	}
	for(int i=0;i<B;i++)
	{
		for(int j=0;j<R;j++)
			delete[] stadium[i][j];
		delete[] stadium[i];
	}
	delete[] stadium;
}


int main()
{
	DoTheThing();

	return 0;
}
