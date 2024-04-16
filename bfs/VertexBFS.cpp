/*
	Think-Like-A-Vertex Framework, VertexBFS.cpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Implementation file for vertices in the breath-first search programs.
	Implements the virtual functions inherited by the VertexBFS.h base class

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#include "gpregellib.h"
#include "VertexBFS.h"

VertexBFS::VertexBFS():Vertex() {}

/*
	How to update the vertex's internal value
	based on the message received.  For BFS,
	we store the smallest value, or the least
	number of hops, received
*/
void VertexBFS::process_message( Message<int>* msg ) {

	int msg_val = (*msg).get_value();
	int v_val = get_value();

	if( msg_val < v_val ) {
		set_value( msg_val );
	}

	return;
}

/*
	How to compute the value to send out to neighbors.
	For BFS, we send out our current number of hops, plus 1
*/
void VertexBFS::send_message( Message<int>* messages_out, int num_messages, int dest_id, int weight ) {

	Message<int> msg;
	msg.set_dest_id(dest_id);
	msg.set_value( get_value()+1 );
	messages_out[num_messages] = msg;

	return;
}
