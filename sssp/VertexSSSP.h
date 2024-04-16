/*
	Think-Like-A-Vertex Framework, VertexSSSP.h
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Single-Source Shortest Path Vertex class header file

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#ifndef VERTEXSSSP_H
#define VERTEXSSSP_H

#include "gpregellib.h"

class VertexSSSP: public Vertex {

	public:

		VertexSSSP();
		
		virtual void process_message( Message<int>* );
		virtual void send_message( Message<int>*, int, int, int );

};

#endif
