/*
	Think-Like-A-Vertex Framework, DynamicArray.h
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>
	
	A dynamic integer array, like a vector for integers, 
	but minimal for the use case, can dynamically 
	allocate memory to expand, or "push_back"

	Has an array of a certain capacity,
	and tracks the number of elements in the array with size.
	when size reaches capacity, the array capacity is doubled
	
	For TLAV, this is used only by the Graph class in dynamically 
	creating arrays to store graph information, not by TLAV programs
*/

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

class DynamicArray {

	public:
		DynamicArray();
		~DynamicArray();
		void push_back(int);
		void expand();
		void print_capacity();
		void print_size();
		int get_size();

		void set(int,int);
		int at(int);
		int* get_array();		

	private:
		int* array;
		int size;
		int capacity;


};

#endif
