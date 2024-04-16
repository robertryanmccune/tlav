/*
	Think-Like-A-Vertex Framework, VertexCC.cpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Connected Component Vertex class implementation file

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#include "gpregellib.h"
#include "VertexCC.h"

VertexCC::VertexCC():Vertex() {}

/*
	How to update the vertex's internal value
	based on the message received. 
	If a value received is less than the node's current value
	update the value to the new value that was received
*/
void VertexCC::process_message( Message<int>* msg ) {

	int msg_val = (*msg).get_value();
	int v_val = get_value();

	if( msg_val < v_val ) {
		set_value( msg_val );
	}

	return;
}

/*
	How to compute the value to send.
	When active, send to neighbors a vert's current value,
	i.e. send your current connected component id 
*/
void VertexCC::send_message( Message<int>* messages_out, int num_messages, int dest_id, int weight ) {

	Message<int> msg;
	msg.set_dest_id(dest_id);
	msg.set_value( get_value() );
	messages_out[num_messages] = msg;

	return;
}
