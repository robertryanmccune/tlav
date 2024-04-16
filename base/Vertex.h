/*
	Think-Like-A-Vertex Framework, Vertex.h
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Header class for a vertex and its function,
	each Vertex must implement 2 virtual functions	

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/
#ifndef VERTEX_H
#define VERTEX_H

#include "gpregellib.h"

class Vertex {

	public:
		Vertex();

		/*
			A vertex type must implement these
			two virtual functions, namely
				- how to process a message, and
				- how to send a message
		*/
		virtual void process_message( Message<int>* ) = 0;
		virtual void send_message( Message<int>*, int, int, int ) = 0;

		int get_id();
		void set_id(int);

		int get_value();
		void set_value(int);

		int* get_neighborstart();
		void set_neighborstart(int*);

		int* get_weightstart();
		void set_weightstart(int*);

		int get_degree();
		void set_degree(int);

		bool get_active();
		void set_active(bool);

	private:
		int id;
		int value;
		int* neighbor_start;
		int* weight_start;
		int degree;
		bool active;

};

#endif
