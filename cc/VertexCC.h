/*
	Think-Like-A-Vertex Framework, VertexCC.h
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Connected Component Vertex class header file

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#ifndef VERTEXCC_H
#define VERTEXCC_H

#include "Vertex.h"

class VertexCC: public Vertex {

	public:

		VertexCC();
		
		// implement the two Vertex virtual functions
		virtual void process_message( Message<int>* );
		virtual void send_message( Message<int>*, int, int, int );

};

#endif
