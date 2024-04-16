/*
	Think-Like-A-Vertex Framework, main_bfs.cpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Driver program for vertex-centric Breadth-First Search.
	Utilizes BFS worker and vertex classes
	
	Input an edgelist and source vertex, and output the 
	number of hops from the source

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/
#include "gpregellib.h"
#include "WorkerBFS.h"
#include "VertexBFS.h"

using namespace std;

/*
	main driver program
*/
int main(int argc, char* argv[]) {

	//inputs
	if( argc != 4 ) {
		printf("usage: ./pregel_bfs  edgelist_in  src  outfile\n");
		return -1;
	}
	
	// arguments to variables
	string graphfile = argv[1];
	int src = atoi(argv[2]);
	string outfile = argv[3];

	printf("Hello World\n");
	printf("Loading graph %s\n", argv[1]);
	
	// initialize worker with graph file	
	WorkerBFS<VertexBFS> worker(graphfile);
	
	// program takes one input argument (the source vert)
	int n_args = 1;

	// package the arguments together
	worker.init_args(n_args, src);
	worker.run();
	worker.write_output(outfile);

	printf("Complete\n");

	return 0;
}
