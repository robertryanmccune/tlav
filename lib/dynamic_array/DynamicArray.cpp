/*
	Think-Like-A-Vertex Framework, DynamicArray.cpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	DynamicArray is like a custom vector-like object for integers
	
	Has an array of a certain capacity,
	and tracks the number of elements in the array with size.
	when size reaches capacity, the array capacity is doubled
	
	For TLAV, only by the graph class in loading graph data
*/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "DynamicArray.h"

/*
	Create a Dynamic Array object,
	with an array of capacity 1,
	and a size of 0, since no elements are added
*/
DynamicArray::DynamicArray() {
	size=0;
	capacity=2;
	array = (int*)calloc( capacity, sizeof(int) );
	for(int i=0; i<2; i++) {
		array[i]=0;
	}
	if(array == NULL) {
		printf("bad initialization of DynamicArray\n");
		exit(1);
	}
}

/*
	Deallocate the array
*/
DynamicArray::~DynamicArray() {

	free(array);

}

/*
	Add an element to the array
*/
void DynamicArray::push_back(int v) {

	if(size == capacity) {
		expand();
	}
	array[size]=v;
	size++;

	return;
}

/*
	Expand the size of the array through reallocution
*/
void DynamicArray::expand() {

	capacity*=2;
	array = (int*)realloc(array, capacity*sizeof(int) );
	if(array == NULL) {
		printf("bad DynamicArray realloc of capacity: %d\n", capacity);
		exit(1);
	}

	return;
}

/*
	return the size of the dynamic array
*/
int DynamicArray::get_size() {
	return size;
}	

/*
	Print the capacity of the array
*/
void DynamicArray::print_capacity() {

	printf("capacity: %d\n",capacity);
	return;
}

/*
	print the size of the array
*/
void DynamicArray::print_size() {

	printf("size: %d\n",size);
	return;
}

/*
	return the value at a given index
*/	
int DynamicArray::at(int i) {
	return array[i];
}

/*
	set the value at a given index
*/
void DynamicArray::set(int i, int v) {
	array[i]=v;
}

/*
	return the underlying array
*/
int* DynamicArray::get_array() {
	return array;
}



