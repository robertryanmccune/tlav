/*
	Think-Like-A-Vertex Framework, Message.h
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Generic templated Message class

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#ifndef MESSAGE_H
#define MESSAGE_H

#include "gpregellib.h"

template <typename T>
class Message {

	public:
		int get_dest_id() { return dest_id; }
		void set_dest_id(int i) { dest_id = i; }

		T get_value() { return value; }
		void set_value(T i) { value = i; }
	
	private:
		int dest_id;
		T value;

};

#endif
