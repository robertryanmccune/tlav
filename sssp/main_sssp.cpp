/*
	Think-Like-A-Vertex Framework, main_sssp.cpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	Driver program for vertex-centric Single Source Shortest Path.
	Utilizes SSPS worker and vertex classes
	
	Input a weighted edgelist and source vertex, and output the 
	distance of each vertex from the source

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#include "gpregellib.h"
#include "WorkerSSSP.h"
#include "VertexSSSP.h"

using namespace std;

int main(int argc, char* argv[]) {

	// arguments include an input and output file, along with a source vertex
	if( argc != 4 ) {
		printf("usage: ./pregel_sssp edgelist_in  src  outfile\n");
		return -1;
	}

	string graphfile = argv[1];
	int src = atoi(argv[2]);
	string outfile = argv[3];

	printf("Hello World\n");
	printf("Loading graph %s\n", argv[1]);
	
	// initialize the worker with a graph
	WorkerSSSP<VertexSSSP> worker(graphfile);

	// only argument required by worker is the source vert
	int n_args = 1;

	worker.init_args(n_args, src);
	worker.run();
	worker.write_output(outfile);

	cout << "sssp execution complete" << endl;

	return 0;
}

