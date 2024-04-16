/* 
	Think-Like-A-Vertex Framework, Graph.h
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>
	
	Header of a Graph object for TLAV program
	Used for the system's internal graph representation 

	Can load graphs in the adjacency or edgelist format
	
	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.

*/

#ifndef GRAPH_H
#define GRAPH_H

#include<string>
using std::string;

class Graph{

	/* constructor, destructor, and public accessor functions */
	public:

		Graph();
		Graph(string);
		~Graph();
		void init(string);
		void init_from_edgelist(string);
		int get_num_verts();
		int get_num_undir_edges();
		int get_num_dir_edges();
		int* get_csr1();
		int* get_csr2();
		int* get_weights();
		int get_degree(int);
		string get_filename();


	/* private functions */
	private:

		void create_adjlist_from_file(string infile);
		void create_csr_from_adjlist();
		void create_csr_from_edgelist();
		void remove_adjlist();
		int read_adjlist_rows(string infile);
		void read_row_neighbors(string infile);
		int parse_row_neighbors(string line);
		void parse_weighted_edgeline(string, int*);
		void read_adjlist(string infile);
		void parse_adjlist_row( string line, int* row );


	/* private variables */
	//private:
		int num_verts;
		int num_edges;
		int* csr1;
		int* csr2;
		int* weights;
		int* num_neighbors;
		int** adjlist;
		string filename;

};

#endif
