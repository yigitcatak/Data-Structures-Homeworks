#ifndef _MY_HEAP_H
#define _MY_HEAP_H
#include <string>
#include <iostream>
using namespace std;



#define MAX_CAPACITY 512

struct memory_block{

	memory_block(): right(NULL), left(NULL), used(true) {};

	memory_block* right; // node to the right
	memory_block* left; // node to the left
	bool used; // if this memory block is reserved or not
	int size; // the number of bytes reserved in Img_heap
	int starting_address; // the starting address in Img_heap
	// of the memory reserved by this block
};

class My_heap{
	private:
		memory_block* heap_begin;
		memory_block* blk;
		int used_bytes;
		// static const int MAX_CAPACITY = 512; // this must remain constant;
		// you don�t need to change it
	public:
		My_heap();
		~My_heap();
		memory_block* bump_allocate(int num_bytes);
		memory_block* first_fit_allocate(int num_bytes);
		memory_block* best_fit_allocate(int num_bytes);
		memory_block* first_fit_split_allocate(int num_bytes);
		void deallocate(memory_block* block_address);
		void print_heap();
		float get_fragmantation();
};

#endif