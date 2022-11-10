#include "my_heap.h"

My_heap::My_heap()
{
	heap_begin = NULL;
	blk = NULL;
	used_bytes = 0;
}

My_heap::~My_heap()
{
	cout << "At destruction, the heap had a memory leak of " << used_bytes << " bytes.\n";

	if(heap_begin == NULL)
		return;

	memory_block *temp = heap_begin, *temp2=heap_begin;
	while(temp->right != NULL)	
	{
		temp = temp->right;
		delete temp2;
		temp2 = temp;
	}	
	delete temp;
}

memory_block* My_heap::bump_allocate(int num_bytes)
{
	if(heap_begin == NULL) 
	{
		memory_block* temp = new memory_block;
		temp->starting_address = 0;
		temp->size = num_bytes;

		heap_begin = temp;
		blk = temp;
		used_bytes += num_bytes;

		return temp;
	}

	if( used_bytes + num_bytes <= MAX_CAPACITY )
	{
		memory_block* temp = new memory_block;
		temp->starting_address = blk->starting_address+blk->size;
		temp->size = num_bytes;

		blk->right = temp;
		temp->left = blk;
		blk = temp;
		used_bytes += num_bytes;

		return temp;
	}

	return NULL;
}

void My_heap::deallocate(memory_block* block_address)
{
	/* 
	As it is not mentioned in document, I did not deleted the merged node even if it is removed from the linked list.
	Left them as they are since they are not deleted and their pointer is set to NULL, user can still try to deallocate them
	as they have access to their pointer by the previously assigned BlockNumber pointer.
	Therefore, following if condition checks for already deallocated ones too
	
	if want to delete the nodes that are taken out of linked list during merging process, uncomment the codes in the code
	*/

	if( block_address == NULL || !block_address->used ) //allocate failed for that block earlier, already deallocated
		return;

	used_bytes -= block_address->size;
	block_address->used = false; // set unused now, only edit connections in following if conditions

	if( block_address != heap_begin )
	{
		if( !block_address->left->used ) //merge with left
		{
			block_address->size += block_address->left->size;
			block_address->starting_address = block_address->left->starting_address;
			
			if( block_address->left == heap_begin ) //if merged node is the new begining
			{
				heap_begin = block_address;

				// UNCOMMENT FOR DELETION OF LOST POINTER IN MERGING
				//delete block_address->left;

				block_address->left = NULL;				
			}

			else
			{
				block_address->left->left->right = block_address;

				// UNCOMMENT FOR DELETION OF LOST POINTER IN MERGING
				//memory_block* temp = block_address->left;

				block_address->left = block_address->left->left;

				// UNCOMMENT FOR DELETION OF LOST POINTER IN MERGING
				//delete temp;
			}
		}
	}

	if( block_address != blk )
	{
		if( !block_address->right->used ) //merge with right
		{
			block_address->size += block_address->right->size;

			if( block_address->right == blk ) //if merged node is the new ending
			{
				blk = block_address;

				// UNCOMMENT FOR DELETION OF LOST POINTER IN MERGING
				//delete block_address->right;

				block_address->right = NULL;
			}

			else
			{
				block_address->right->right->left = block_address;

				// UNCOMMENT FOR DELETION OF LOST POINTER IN MERGING
				//memory_block* temp = block_address->right;

				block_address->right = block_address->right->right;

				// UNCOMMENT FOR DELETION OF LOST POINTER IN MERGING
				//delete temp;
			}
		}
	}
}

void My_heap::print_heap()
{
	cout << "Maximum capacity of heap: "<< MAX_CAPACITY<< "B\n";
	cout << "Currently used memory (B): "<< used_bytes << endl;

	//////////////// Count Nodes //////////////////
	int UsedCounter=0,UnusedCounter=0;
	memory_block* temp = heap_begin;
	if(heap_begin != NULL)
	{
		while(temp->right != NULL)
		{
			if(temp->used)
				UsedCounter++;
			else
				UnusedCounter++;
			temp=temp->right;
		}
		if(temp->used)
			UsedCounter++;
		else
			UnusedCounter++;
	}
	//////////////////////////////////////////////

	cout << "Total memory blocks: " << UsedCounter + UnusedCounter << endl;
	cout << "Total used memory blocks: " << UsedCounter << endl;
	cout << "Total free memory blocks: " << UnusedCounter << endl;
	cout << "Fragmentation: " << get_fragmantation() << "%\n" ;
	cout << "------------------------------\n";

	////// Iterating over the list again  //////
	if(heap_begin != NULL)
	{
		UsedCounter = 0; 
		string Used[2] =   {"True","False"};
		temp = heap_begin; //re-using these instead of new counter
		while(temp->right != NULL)
		{
			if(temp->used)
				cout << "Block " << UsedCounter << "\t\tUsed: " << Used[0] << "\tSize (B): " << temp->size << "\tStarting Address: 0x" << hex << temp->starting_address << dec << endl;
			else
				cout << "Block " << UsedCounter << "\t\tUsed: " << Used[1] << "\tSize (B): " << temp->size << "\tStarting Address: 0x" << hex << temp->starting_address << dec << endl;
			UsedCounter++;
			temp=temp->right;
		}
		if(temp->used)
			cout << "Block " << UsedCounter << "\t\tUsed: " << Used[0] << "\tSize (B): " << temp->size << "\tStarting Address: 0x" << hex << temp->starting_address << dec << endl;
		else
			cout << "Block " << UsedCounter << "\t\tUsed: " << Used[1] << "\tSize (B): " << temp->size << "\tStarting Address: 0x" << hex << temp->starting_address << dec << endl; 
	}
	////////////////////////////////////////////
	cout << "------------------------------\n------------------------------\n";
}

float My_heap::get_fragmantation()
{
	if(heap_begin == NULL)
		return 0;

	float free_memory = MAX_CAPACITY - used_bytes;

	float biggest_free_block= MAX_CAPACITY - (blk->starting_address + blk->size);

	memory_block* temp = heap_begin;
	while(temp->right != NULL)
	{
		if( !temp->used && temp->size > biggest_free_block )
			biggest_free_block = temp->size;
		temp = temp->right;
	}
	if( !temp->used && temp->size > biggest_free_block )
		biggest_free_block = temp->size;

	float fragmentation = (free_memory - biggest_free_block)*100/free_memory;
	return fragmentation;
}

memory_block* My_heap::first_fit_allocate(int num_bytes)
{
	if(heap_begin == NULL) // if list is empty it has to use bump allocate
		return bump_allocate(num_bytes);


	//////////////////////////// Iteration over list ///////////////////////////////////////
	memory_block *temp = heap_begin;
	while(temp->right != NULL)
	{
		if(!temp->used && temp->size >= num_bytes)
		{
			temp->used = true;
			used_bytes += temp->size;
			return temp;
		}
		temp = temp->right;
	}
	if(!temp->used && temp->size >= num_bytes)
	{
		temp->used = true;
		used_bytes += temp->size;
		return temp;
	}
	////////////////////////////////////////////////////////////////////////////////////////


	return bump_allocate(num_bytes); // if nothing could have returned up to this point
	// we need to bump allocate or there is no space which bump_allocate also handles
}

memory_block* My_heap::best_fit_allocate(int num_bytes)
{
	if(heap_begin == NULL) // if list is empty it has to use bump allocate
		return bump_allocate(num_bytes);


	//////////////////////////// Iteration over list ///////////////////////////////////////
	memory_block *temp = heap_begin, *temp2=NULL;
	while(temp->right != NULL)
	{
		if(!temp->used && temp->size >= num_bytes)
		{
			if( temp2 != NULL && temp->size < temp2->size )
				temp2 = temp;
			else //first fitting will be automatically assigned 
				temp2 = temp;
		}
		temp = temp->right;
	}
	if(!temp->used && temp->size >= num_bytes)
	{
		if( temp2 != NULL && temp->size < temp2->size )
			temp2 = temp;
		else
			temp2 = temp;
	}
	////////////////////////////////////////////////////////////////////////////////////////
	

	if(temp2 == NULL) // no proper space exists in between fragmented nodes
		return bump_allocate(num_bytes);
	
	temp2->used = true;
	used_bytes += temp2->size;
	return temp2;
}

memory_block* My_heap::first_fit_split_allocate(int num_bytes)
{
	if(heap_begin == NULL) // if list is empty it has to use bump allocate
		return bump_allocate(num_bytes);

	
	//////////////////////////// Iteration over list ///////////////////////////////////////
	memory_block *temp = heap_begin;
	while(temp->right != NULL)
	{
		if(!temp->used)
		{
			if(temp->size == num_bytes)
			{
				temp->used = true;
				used_bytes += num_bytes;
				return temp;
			}

			if(temp->size > num_bytes)
			{
				memory_block *temp2 = new memory_block;
				temp2->size = temp->size - num_bytes;
				temp2->starting_address = temp->starting_address + num_bytes;
				temp2->right = temp->right;
				temp2->left = temp;
				temp2->used = false;

				
				if(temp == blk)
					blk = temp2;
				else
					temp->right->left = temp2;

				temp->size = num_bytes;
				temp->right = temp2;
				temp->used = true;
				

				used_bytes += num_bytes;
				return temp;
			}
		}
		temp = temp->right;
	}
	//////////////////////////// LOOP ENDED, CHECK LAST //////////////////////////////////////
	if(!temp->used)
	{
		if(temp->size == num_bytes)
		{
			temp->used = true;
			used_bytes += num_bytes;
			return temp;
		}

		if(temp->size > num_bytes)
		{
			memory_block *temp2 = new memory_block;
			temp2->size = temp->size - num_bytes;
			temp2->starting_address = temp->starting_address + num_bytes;
			temp2->right = temp->right;
			temp2->left = temp;
			temp2->used = false;

			if(temp == blk)
					blk = temp2;
			else
				temp->right->left = temp2;

			temp->right = temp2;
			temp->used = true;
			temp->size = num_bytes;

			used_bytes += num_bytes;
			return temp;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////


	return bump_allocate(num_bytes); // again if nothing returned yet bump allocate will handle
}


