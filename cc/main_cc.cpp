/*
	Think-Like-A-Vertex Framework, main_cc.cpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Driver program for vertex-centric Connected Components
	
	Takes in an edgelist and outputs the connected
	component of each vertex.  The id of the connected 
	component is the smallest vert_id among vertices in a component.

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#include "gpregellib.h"
#include "WorkerCC.h"
#include "VertexCC.h"

using namespace std;

int main(int argc, char* argv[]) {

	//arguments include edgelist in and the string of the file to be outputted
	if( argc != 3 ) {
		printf("usage: ./pregel_cc  edgelist_in  outfile\n");
		return -1;
	}

	string graphfile = argv[1];
	string outfile = argv[2];

	printf("Loading graph %s\n", argv[1]);
	
	// initialize the graph structure in the worker
	WorkerCC<VertexCC> worker(graphfile);
	
	// no input arguments required
	int n_args = 0;

	worker.init_args(n_args);
	worker.run();
	worker.write_output(outfile);

	printf("Complete\n");

	return 0;
}
