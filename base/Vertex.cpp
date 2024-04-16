/*
	Think-Like-A-Vertex Framework, Vertex,cpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Implementation of some of the generic Vertex functions

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#include "gpregellib.h"

Vertex::Vertex() { }

/*
	implementation of get and set methods
*/
int Vertex::get_id() { return id; }
void Vertex::set_id(int i) { id = i; }

int Vertex::get_value() {	return value; }
void Vertex::set_value(int i) { value = i; }

int* Vertex::get_neighborstart() { return neighbor_start; }
void Vertex::set_neighborstart(int* ns) { neighbor_start = ns; }

int* Vertex::get_weightstart() { return weight_start; }
void Vertex::set_weightstart(int* ws) { weight_start = ws; }

int Vertex::get_degree() { return degree; }
void Vertex::set_degree(int i) { degree = i; }

bool Vertex::get_active() { return active; }
void Vertex::set_active(bool act) {	active = act; }
