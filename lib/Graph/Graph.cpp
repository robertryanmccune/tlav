/* 
	Think-Like-A-Vertex Framework, Graph.cpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>
	
	Implementation of a Graph object for TLAV program
	Used for underlying graph representation 

	Can load graphs in the adjacency or edgelist format
	
	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#include<fstream>
using std::ifstream;

#include<cstdlib>
using std::atoi;

#include<iostream>
using std::cout;
using std::endl;

#include "string.h"

#include "DynamicArray.h"
#include "Graph.h"

/* Constructor */
Graph::Graph() {

}

/*
	Initiate a graph given the string of a graphfile
	Graph file assumed to be stored in adjacency list format.
	Read the graph and store it as an adjacency list,
	then convert the internal adjacency list into 
	the compressed sparse row format
*/
Graph::Graph(string infile) {

	filename = infile;
	create_adjlist_from_file(filename);
	create_csr_from_adjlist();
	remove_adjlist();

}

/* Destructor */
Graph::~Graph() {
	delete [] csr1;
	delete [] csr2;
}

/*
	Initiate a graph given the string of a graphfile
	Graph file assumed to be stored in adjacency list format.
	Read the graph and store it as an adjacency list,
	then convert the internal adjacency list into 
	the compressed sparse row format
	
	Same as constructing a graph with an input file
*/
void Graph::init(string infile) {

	filename = infile;
	create_adjlist_from_file(filename);
	create_csr_from_adjlist();
	remove_adjlist();

}

/*
	Initialize a graph given a
	graphfile written in the edgelist format
*/
void Graph::init_from_edgelist(string infile) {
	filename = infile;
	create_csr_from_edgelist();
}

/* num verts accessor */
int Graph::get_num_verts() {
	return num_verts;
}

/* num undirected edges accessor */
int Graph::get_num_undir_edges() {
	return (num_edges/2);
}

/* num directed edges accessor */
int Graph::get_num_dir_edges() {
	return num_edges;
}

/*
	return the first array of the CSR format
*/
int* Graph::get_csr1() {
	return csr1;
}

/*
	return the second array of the CSR format
*/
int* Graph::get_csr2() {
	return csr2;
}

/*
	return the third array of the CSR format
	storing weights
*/
int* Graph::get_weights() {
	return weights;
}	

/*
	return the degree of a given node
*/
int Graph::get_degree(int v) {
	return csr1[v+1] - csr1[v];
}

/*
	return the name of the graph input file
*/
string Graph::get_filename() {
	return filename;
}

/***** Private functions *****/

/*
	Create an adjacency list representation
	of the inputted graph file,
	assuming the graph is stored in adjacency list format
	
	adjacency list stored in 2-D array, where each
	index stores that verice's neighbors
*/
void Graph::create_adjlist_from_file(string infile) {

	num_verts = read_adjlist_rows(infile);
	num_neighbors= new int[num_verts];
	adjlist=new int*[num_verts];
	read_row_neighbors(infile);
	int row_size;
	num_edges=0;
	for(int i=0; i<num_verts; i++) {
		row_size = num_neighbors[i];
		adjlist[i]=new int[row_size];
		num_edges+=row_size;
	}
	read_adjlist(infile);
	return;
}

/*
	Create a Compressed Sparse Row formmatted
	graph representation from the 
	adjacency list representation
*/
void Graph::create_csr_from_adjlist() {
	csr1=new int[num_verts+1];
  csr2=new int[num_edges];
	int row_size=0;
	int edge_count=0;
	csr1[0]=0;
	for(int i=0; i<num_verts; i++) {
		row_size=num_neighbors[i];
		for(int j=0; j<row_size; j++) {
			csr2[edge_count] = adjlist[i][j];
			edge_count++;
		}
		csr1[i+1]=edge_count;
	}
	return;

}

/* 
	create the CSR graph representation 
	from an edgelist file.
	Utilized the fact that the sorted edgelist
	can smoothly create the CSR arrays
*/
void Graph::create_csr_from_edgelist() {

	ifstream infile;
	infile.open(filename.c_str());
	string line;

	DynamicArray n1,n2,w;
	int edge[3];
	int edgecount=0, v1=-1;
	int largest_node_label = -1;
	int last_v1;

	while( getline(infile, line) ) {
		parse_weighted_edgeline(line, edge);
		if( v1 != edge[0] ) {
			n1.push_back(edgecount);
			v1 = edge[0];
		}
		n2.push_back(edge[1]);
		w.push_back(edge[2]);	
		edgecount++;

		if( edge[1] > largest_node_label ) {
			largest_node_label = edge[1];
		}
	}
	last_v1 = v1;
	n1.push_back(edgecount);


	// account for large-labeled nodes w/ no out-edges
	int num_extra_nodes = largest_node_label - last_v1;
	for(int i=0; i<num_extra_nodes; i++) {
		n1.push_back(edgecount);
	}

	num_verts = n1.get_size() - 1;
	num_edges = n2.get_size();

	csr1 = new int[num_verts +1];
	csr2 = new int[num_edges];
	weights = new int[num_edges];

	memcpy( csr1, n1.get_array(), sizeof(int) * (num_verts+1) );
	memcpy( csr2, n2.get_array(), sizeof(int) * num_edges );
	memcpy( weights, w.get_array(), sizeof(int) * num_edges );

	return;
}

/*
	Deallocate the adjacency list representation
*/	
void Graph::remove_adjlist() {
	for(int i=0; i<num_verts; i++)
		delete [] adjlist[i];
	delete [] adjlist;
	delete [] num_neighbors;
	return;
}

/*
	Get the number of rows in an adjacency list file
*/
int Graph::read_adjlist_rows(string infile) {
	ifstream instream;
	instream.open(infile.c_str());
	string line;
	int num_rows=0;
	while( getline( instream, line) )
		num_rows++;
	instream.close();
	return num_rows;
}

/*
	Get the number of neighbors for each vertex
	in an adjacency list
*/	
void Graph::read_row_neighbors(string infile) {
	ifstream instream;
	instream.open( infile.c_str() );
	string line;
	int row=0;
	int neighbor_count=0;
	while( getline( instream, line) ) {
		neighbor_count=parse_row_neighbors( line );	
		num_neighbors[row]=neighbor_count;
		row++;
	}
	instream.close();
	return;
}

/*
	parse the given line of an adjacency list
	assumes space delimitted
*/
int Graph::parse_row_neighbors( string line ) {
	
	int neighbor_count=0;
	bool parsed_v=false;
	for(int i=0; i<line.size(); i++) {
		if( line[i] == ' ' ) {
			if(parsed_v==false) {
				parsed_v=true;
			} else {
				neighbor_count++;
			}
		}		
	}
	neighbor_count++;

	return neighbor_count;
}

/* searches for the 2 tabs in a weighted edgelist file */
void Graph::parse_weighted_edgeline( string edgeline, int* edge ) {

	// unweighted
	if(edgeline.find_first_of('\t') == edgeline.find_last_of('\t') ) {
		int t1 = edgeline.find_first_of('\t');
		
		edge[0] = atoi( edgeline.substr(0,t1).c_str() );
		edge[1] = atoi( edgeline.substr(t1+1, edgeline.length()-t1-1 ).c_str() );
		edge[2]=1;
	} else { // weighted 
		int t1 = edgeline.find_first_of('\t');
		int t2 = edgeline.find_last_of('\t');
		edge[0]=atoi( edgeline.substr(0,t1).c_str() );
		edge[1]=atoi( edgeline.substr(t1+1, t2-t1-1).c_str() );
		edge[2]=atoi( edgeline.substr(t2+1, edgeline.length()-t2-1).c_str() );
	}
	
	return;
}

/*
	open an adjacency graph file and 
	parse each row into the adjacency list
*/
void Graph::read_adjlist(string infile) {

	ifstream instream;
	instream.open( infile.c_str() );
	string line;
	int row_count=0;
	while( getline( instream, line ) ) {
		parse_adjlist_row( line, adjlist[row_count] );
		row_count++;
	}

	return;
}

/*
	parse each row of the adjacency list,
	placing the neighbors in the file into
	each entry in the internal adjacency list
*/
void Graph::parse_adjlist_row( string line, int* row ) {

	string vert_str;
	int start, length;
	bool parsed_v=false;
	start=0;
	int vert;
	int vert_count=0;
	for(int i=0; i<line.size(); i++) {
		if( line[i] == ' ' ) {
			length=i-start;
			vert_str = line.substr( start , length );
			if(parsed_v==false) {
				parsed_v=true;
			} else {
				vert = atoi(vert_str.c_str() );
				row[vert_count] = vert;
				vert_count++;
			}
			start=i+1;
		}		
	}
	length=line.size()-start;
	vert_str = line.substr( start , length );
	vert = atoi(vert_str.c_str() );
	row[vert_count] = vert;

	return;
}
