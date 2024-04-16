/*
	Think-Like-A-Vertex Framework, VertexSSSP.cpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Single-source Shortest Path Vertex class implementation file

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#include "gpregellib.h"
#include "VertexSSSP.h"

VertexSSSP::VertexSSSP():Vertex() {}

/*
	How to update the vertex's internal value
	based on the message received
*/
void VertexSSSP::process_message( Message<int>* msg ) {

	int msg_val = msg->get_value();
	int v_val = get_value();

	if( msg_val < v_val ) {
		set_value( msg_val );
	}

	return;
}

/*
	How to compute the value to send 
*/
void VertexSSSP::send_message( Message<int>* messages_out, int num_messages, int dest_id, int weight ) {

	Message<int> msg;
	msg.set_dest_id(dest_id);
	msg.set_value( get_value() + weight );
	messages_out[num_messages] = msg;

	return;
}
