/*
	Think-Like-A-Vertex Framework, WorkerCC.h
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Worker class for Connected Components
	Inherits base Worker class functions

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#ifndef WORKERCC_H
#define WORKERCC_H

#include "gpregellib.h"

template <typename T>
class WorkerCC: public Worker<T> {

	public:
		WorkerCC(string);
		void init_args(int, ...);
		void write_output(string);

};

/* Constructor */
template<typename T>
WorkerCC<T>::WorkerCC(string infile):Worker<T>(infile) {}

/*
	How to initialize vertices at the start of the program.
	No input (or source vertex) is required, but list
	every vertex as active
*/
template<typename T>
void WorkerCC<T>::init_args(int num_args, ...) {
	

	T* vert_cont = this->get_vert_cont();
	int num_verts = this->get_num_verts();
	T* vert;
	for(int i=0; i<num_verts; i++) {
		vert = &vert_cont[i];
		vert->set_value( vert->get_id() );
		vert->set_active(true);
	}
	this->set_vert_cont(vert_cont);

	return;
}

/*
	write to a file each node and the value it holds.
	The value is the id of its connected component
*/
template<typename T>
void WorkerCC<T>::write_output(string outfile) {

	ofstream outstream;
	outstream.open( outfile.c_str() );

	T vert, *vert_cont;
	int value, id;
	outstream << "v_id\tcomponent" << endl;
	vert_cont = this->get_vert_cont();
	for(int i=0; i<this->get_num_verts(); i++) {
		vert = vert_cont[i];
		value = vert.get_value();
		id = vert.get_id();
		outstream << id << "\t" << value << endl;
	}

	outstream.close();

	return;
}

#endif
