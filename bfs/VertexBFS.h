/*
	Think-Like-A-Vertex Framework, VertexBFS.h
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Header class for vertices in the breath-first search programs.
	Inherits the virtual functions from the base class Vertex.h

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#ifndef VERTEXBFS_H
#define VERTEXBFS_H

#include "Vertex.h"

class VertexBFS: public Vertex {

	public:

		VertexBFS();
		
		virtual void process_message( Message<int>* );
		virtual void send_message( Message<int>*, int, int, int );

};

#endif
