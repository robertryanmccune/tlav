/*
	Think-Like-A-Vertex Framework, WorkerBFS.h
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Worker class for breadth-first search
	Inherits base Worker class functions

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/


#ifndef WORKERSSSP_H
#define WORKERSSSP_H

#include "Worker.h"
#include "gpregellib.h"

template <typename T>
class WorkerBFS: public Worker<T> {

	public:
		WorkerBFS(string);
		void init_args(int, ...);
		void write_output(string);


};

/* Constructor */
template<typename T>
WorkerBFS<T>::WorkerBFS(string infile):Worker<T>(infile) {}

/*
	Read the arguments to the Worker evocations
	Gets the bfs source node input by the user
	and initializes it by setting the node's 
	value to 0 and marking it as active
*/
template<typename T>
void WorkerBFS<T>::init_args(int num_args, ...) {
	
	va_list list;
	va_start(list, num_args);
	int src = va_arg(list, int);
	cout << "assigning src vert: " << src << endl;
	va_end(list);

	if( src < 0 || src >= this->get_num_verts() ) {
		cout << "error: src out of range" << endl;
		exit(-1);
	}

	T* vert_cont = this->get_vert_cont();
	int num_verts = this->get_num_verts();
	T* vert;
	for(int i=0; i<num_verts; i++) {
		vert = &vert_cont[i];
		if( i==src) {
			vert->set_value(0);
			vert->set_active(true);
		} else {
			vert->set_value(INT_MAX);
			vert->set_active(false);
		}
	}
	this->set_vert_cont(vert_cont);

	return;
}

/*
	Write the program output to a file
	entering each node and it's number of hops
	from the source node.  If a node is unreached,
	print that as so.
*/
template<typename T>
void WorkerBFS<T>::write_output(string outfile) {

	ofstream outstream;
	outstream.open( outfile.c_str() );

	T vert, *vert_cont;
	int value, id;
	outstream << "v_id\tlevel" << endl;
	for(int i=0; i<this->get_num_verts(); i++) {
		vert_cont = this->get_vert_cont();
		vert = vert_cont[i];
		value = vert.get_value();
		id = vert.get_id();
		if( value == INT_MAX ) {
			outstream << id << "\t" << "unreached" << endl;
		}	else {	
			outstream << id << "\t" << value << endl;
		}
	}

	outstream.close();

	return;
}

#endif
