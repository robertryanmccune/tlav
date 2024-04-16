/*
	Think-Like-A-Vertex Framework, edgelist_preprocessing.cpp
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>
	
	Processes edgelists for use in the TLAV system.
	
	Example edgeslists can be those generated by the SNAP package.
	This program processes these edgeslists to work
	with the TLAV system.
	
	Edgelists from generators are not immediately ready for 
	use here with TLAV.  For example, they may have self-loops
	or non-contiguous labels.  This preprocessing program
	makes sure the edgelists will work  with TLAV functions.  
		
	Also, while edges from generators are typically unweighted, this program
	offers to add random weights between 1 and 100 if wanted,
	for example to use with single source shortest path.

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

/* function prototypes */
void read_edgelist(string, std::vector<pair<int, int> >&);
pair<int,int> parse_edge(string);
void edgelist_remove_duplicates(vector<pair<int, int> >&);
void edgelist_remove_self_loops(vector<pair<int, int> >&);
void convert_edgelist_to_adjlist(vector<pair<int, int> >&, map<int,vector<int> >&);
void adjlist_remove_reverse_edges(map<int,vector<int> >&);
void adjlist_sort_rows( map<int, vector<int> >& );
void convert_adjlist_map_to_vec( map<int, vector<int> >&, vector<pair<int, vector<int> > >& );
void adjlist_relabel_verts_index_0( vector<pair<int, vector<int> > >& );
void convert_adjlist_to_edgelist( vector< pair<int, vector<int> > >&, vector< pair<int,int> >&);
void add_weights_to_edgelist( vector< pair<int,int> >&, vector< tuple<int,int,int> >& );
void make_edgelist_undirected(vector<pair<int, int> >&);
void make_wedgelist_undirected( vector< tuple<int,int,int> >&);
void print_edges( vector<pair<int, vector<int> > >& );
void write_edgelist( vector<pair<int, vector<int> > >&, int*, int, string);
void write_wedgelist( vector<tuple<int,int,int> >&, string);
void write_unweighted_edgelist( vector< pair<int,int> >&, string);

/*
	Edgelist preprocessing main program
*/
int main(int argc, char* argv[]) {

	if(argc !=4) {
		cout << "usage:  ./edge_to_edge   edgelist_in   edgelist_out   weighted" << endl;
		return -1;
	}

	string filename = argv[1];
	string outfile = argv[2];
	int weighted = atoi(argv[3]);
	if( weighted!=0 && weighted != 1) {
		cout << "error: weighted parameter must be 0 or 1" << endl;
		return -1;
	}
	
	/*
		initialize an edgelist and two adjacency list
		graph representations.  Some formats are easier
		to preprocess than others, depending on the operations
	*/
	vector<pair<int, int> > edges;
	map<int, vector<int> > adjlist1;
	vector<pair<int, vector<int> > > adjlist2;

	/* read in the edgelist into an edge vector */
	read_edgelist(filename, edges);
	
	/* remove duplicate edges and self loops */
	edgelist_remove_duplicates(edges);
	edgelist_remove_self_loops(edges);
	
	/* 
		convert to an adjacency list and
		remove all reverse edges
	*/
	convert_edgelist_to_adjlist(edges, adjlist1);
	adjlist_remove_reverse_edges(adjlist1);

	/*
		sort the rows of the adjacency list
		then relabel the vertices to make
		sure the vert_ids are continuous
	*/
	adjlist_sort_rows(adjlist1);
	convert_adjlist_map_to_vec(adjlist1, adjlist2);
	adjlist_relabel_verts_index_0(adjlist2);
	
	/*
		convert the adjacency list to an edgelist
	*/
	convert_adjlist_to_edgelist( adjlist2, edges);
	
	
	/*
		it the user opts to not add edgeweights
		make edges go both ways, sort them,
		then write to a file
	*/
	if(weighted==0) {
		make_edgelist_undirected(edges);
		sort(edges.begin(), edges.end() );
		write_unweighted_edgelist(edges, outfile);
		return 0;
	}
	
	/* 
		if weighted is wanted, then
		create a collection of weighted edgeweights (3 datums),
		generate random weights for each edges, 
		then add reverse edges, sort, and 
		write to an outputted edgelist
	*/
	vector< tuple<int,int,int> > wedges;
	add_weights_to_edgelist( edges, wedges );
	make_wedgelist_undirected( wedges );
	sort(wedges.begin(), wedges.end());
	write_wedgelist( wedges, outfile);

	return 0;

}


/*
	reads an edgelist from file into a vector of integer pairs
*/
void read_edgelist(string filename, std::vector<pair<int, int> >& edge_vec) {

	std::ifstream edgelist;
	edgelist.open(filename.c_str());
	string line;

	pair<int,int> edge;

	int count=0;
	while( getline(edgelist, line) ) {
		count++;
		if(line[0]=='#') { //ignore comments at top of graphs from SNAP
			continue;
		}
		edge=parse_edge(line);
		edge_vec.push_back(edge);
	}

	edgelist.close();
	cout << "read " << count << " lines" << endl;
	cout << edge_vec.size() << " edges" << endl;
	return;
}

/*
	Sub-program used by read_edgelist() for parsing row of edgelist.
	Assumes tab-delimitation.
*/
pair<int,int> parse_edge(string edge_str) {

	pair<int,int> edge;
	int v1,v2;
	int pos=edge_str.find("\t");
	v1=atoi( edge_str.substr(0,pos).c_str() );
	v2=atoi( edge_str.substr(pos+1,edge_str.length()).c_str() );
	edge= make_pair(v1,v2);

	return edge;
}


/*
	add each edge from the edgelist into a *set* of edges
	the set can only contain each edge once,
	so edges cannot be duplicated
*/
void edgelist_remove_duplicates(vector<pair<int, int> >& edgelist) {
	set<pair<int, int> > edgelist_sans_duplicates;
	pair< set<pair<int, int> >::const_iterator, bool> result_pair;
	int num_duplicates=0;

	vector<pair<int, int> >::iterator edge_itr;
	
	for(edge_itr=edgelist.begin(); edge_itr!=edgelist.end(); ++edge_itr) {
		result_pair = edgelist_sans_duplicates.insert( *edge_itr );
		if(!result_pair.second) num_duplicates++;
	}	

	set<pair<int, int> >::iterator set_itr;
	edgelist.clear();
	for(set_itr=edgelist_sans_duplicates.begin(); set_itr!=edgelist_sans_duplicates.end(); ++set_itr)
		edgelist.push_back(*set_itr);

	cout << num_duplicates << " duplicate edges removed" << endl;
	return; 
}

/*
	Remove self-loops from an edgelist.  Iterates through edgelist
	to find edge with equal first and second node.
*/	
void edgelist_remove_self_loops(vector<pair<int, int> >& edgelist) { 
	int num_self_loops=0;
	vector<pair<int, int> >::iterator edge_itr;
	vector<pair<int, int> > edgelist_no_loops;

	for(edge_itr= edgelist.begin(); edge_itr!= edgelist.end(); ++edge_itr) {
		if ( (*edge_itr).first == (*edge_itr).second )
			num_self_loops++;
		else
			edgelist_no_loops.push_back( (*edge_itr) );
	}
	
	edgelist=edgelist_no_loops;
	cout << num_self_loops << " self loops removed" << endl;
}

/*
	Converts an edgelist into an adjacency list, by
	taking edge's first and second node, seeing if the
	adjlist contains an entry row for the first node...	
	if not, create a new row, add the second node, add the row;
	if so, add the second node (b/c no duplicates assumed in edgelist)
*/
void convert_edgelist_to_adjlist(vector<pair<int, int> >& edgelist, map<int,vector<int> >& adjlist) {

	vector<pair<int, int> >::iterator edge_itr;
	int v1,v2;
	int count=0;
	for(edge_itr=edgelist.begin(); edge_itr!=edgelist.end(); ++edge_itr) {
		v1=(*edge_itr).first;
		v2=(*edge_itr).second;
		if( adjlist.find(v1) == adjlist.end() ) {
			vector<int> row;
			row.push_back(v2);
			adjlist.insert( make_pair(v1, row) );
		} else {
			vector<int> row=(*(adjlist.find(v1))).second;
			row.push_back(v2);
			adjlist.erase(v1);
			adjlist.insert( make_pair(v1,row) );
		}
		count ++;
		//if(count%1000==0) cout << count << endl;
	}
	edgelist.clear();

	cout << count << " edges converted to adjlist" << endl;

	return;
}

/*
	for each edge, find if a reverse exists
	if so, remove the reverse, and keep the "first" edge found
*/
void adjlist_remove_reverse_edges(map<int, vector<int> >& adjlist) { 

	int num_reverse_edges_removed=0;
	map<int,vector<int> >::iterator adj_itr;
	for(adj_itr=adjlist.begin(); adj_itr!=adjlist.end(); ++adj_itr) {
		int v=(*adj_itr).first;
		vector<int> v_adj= adjlist[v];
		vector<int>::iterator v_row_itr;
		for(v_row_itr=v_adj.begin(); v_row_itr!=v_adj.end(); ++v_row_itr) {
			int u=*v_row_itr;
			if( adjlist.find(u) == adjlist.end() ) continue;
			else {
				vector<int> u_adj = adjlist[u];
				vector<int>::iterator u_row_itr;
				for(u_row_itr=u_adj.begin(); u_row_itr!=u_adj.end(); ++u_row_itr) {
					if(*u_row_itr == v) {
						num_reverse_edges_removed++;
						vector<int> row_with_v = adjlist[u];
						row_with_v.erase( find(row_with_v.begin(), row_with_v.end(), v) );
						vector<int> row_without_v = row_with_v;
						adjlist.erase(u);
						adjlist.insert( make_pair( u, row_without_v) );
					}
				}
			}
		}
	}

	cout << num_reverse_edges_removed << " reverse edges removed" << endl;
	return;
}

/*
	sort each row in an adjacency list. uses stl sort
*/
void adjlist_sort_rows( map<int, vector<int> >& adjlist) {
	int num_rows_sorted=0;
	for( map<int, vector<int> >::iterator adj_itr = adjlist.begin(); adj_itr!= adjlist.end(); ++adj_itr) {
		sort( ((*adj_itr).second).begin(), ((*adj_itr).second).end() );
		num_rows_sorted++;
	}
	cout << num_rows_sorted << " rows of adjlist sorted" << endl;
	return;
}

/*
	Convert and adjacency list that uses a map
	to an adjacency list that uses a vector.
	Makes some operations easier.
*/
void convert_adjlist_map_to_vec( map<int, vector<int> >& adjlist, vector<pair<int, vector<int> > >& adjlist_vec) {
	for( map<int, vector<int> >::iterator adj_itr=adjlist.begin(); adj_itr!= adjlist.end(); ++adj_itr) {
		adjlist_vec.push_back( make_pair( (*adj_itr).first, (*adj_itr).second) );
	}
	adjlist.clear();
	cout << adjlist_vec.size() << " verts in new adjlist" << endl;
	return;
}

/*
	relabel an adjacency list so vertex labels are sequential from 0
*/
void adjlist_relabel_verts_index_0( vector<pair<int, vector<int> > >& adjlist) {

	map<int, int> relabels;
	int relabel_count =0;
	int	total_verts_relabeled =0;
	
	vector<pair<int, vector<int> > >::iterator itr;
	int n1, n2;
	vector<int> row;
	for( itr = adjlist.begin(); itr != adjlist.end(); ++itr) {
		n1 = (*itr).first;
		if( relabels.find(n1) == relabels.end() ) {
			relabels.insert( make_pair( n1, relabel_count ) );
			relabel_count++;
		}	
	}

	for( itr = adjlist.begin(); itr != adjlist.end(); ++itr) {
		row = (*itr).second;
		for(int i=0; i<row.size(); i++) {
			n2 = row[i];
			if( relabels.find(n2) == relabels.end() ) {
				relabels.insert( make_pair( n2, relabel_count ) );		
				relabel_count++;
			}
		}		
	}


	// relabel all verts
	for( int i=0;  i<adjlist.size(); i++) {		
		adjlist[i].first = relabels[ adjlist[i].first ];
		total_verts_relabeled++;
		for( int j=0; j< (adjlist[i].second).size(); j++ ) {
			(adjlist[i].second)[j] = relabels[ (adjlist[i].second)[j] ];
			total_verts_relabeled++;
		}
		sort( (adjlist[i].second).begin(), (adjlist[i].second).end() );
	}

	cout << "relabeled " << total_verts_relabeled << " verts" << endl;
	return;
}

/* 
	convert a vector-based adjacency list to a vector of edges
*/
void convert_adjlist_to_edgelist( vector<pair<int, vector<int> > >& adjlist, vector< pair<int,int> >& edgelist) {
	
	edgelist.clear();
	int edges_processed=0;

	vector< pair<int, vector<int> > >::iterator adj_itr;
	vector<int>::iterator row_itr;

	for(adj_itr=adjlist.begin(); adj_itr!=adjlist.end(); ++adj_itr) {
		for(row_itr=((*adj_itr).second).begin(); row_itr!=((*adj_itr).second).end(); ++row_itr) {
			edgelist.push_back( make_pair( (*adj_itr).first, *row_itr) );
			edges_processed++;
		}
	}

	adjlist.clear();
	cout << edges_processed << " edges coverted to edgelist" << endl;
	return;
}

/* 
	from a vector of edges create a vector of weighted edges
	with random edge weights
*/
void add_weights_to_edgelist( vector< pair<int,int> >& edges, vector< tuple<int,int,int> >& wedges) {

	vector< pair<int,int> >::iterator edge_itr;
	int v1, v2, weight;
	srand( (unsigned)time(NULL) );
	
	for(edge_itr = edges.begin(); edge_itr != edges.end(); ++edge_itr) {
		v1 = (*edge_itr).first;
		v2 = (*edge_itr).second;
		weight = rand() % 100;
		tuple<int,int,int> wedge(v1,v2,weight);
		wedges.push_back(wedge);
	}

	return;
}

/*
	make edgelists "undirected", meaning that in the 
	graph representation, whether an adjacency list or edgelist,
	if an edge (u,v) exists then there is also an edge (v,u).
	Kind of similar to a directed graph where edges always
	exist in both directions between 2 nodes
*/
void make_edgelist_undirected(vector<pair<int, int> >& edges) {

	vector< pair<int,int> >::iterator edge_itr;
	vector< pair<int,int> > reverse_edges;
	pair<int,int> edge, reverse_edge;
	int v1, v2;
	
	for( edge_itr = edges.begin(); edge_itr != edges.end(); ++edge_itr) {
		edge=*edge_itr;
		v1=edge.first;
		v2=edge.second;
		reverse_edge=make_pair(v2, v1);
		reverse_edges.push_back(reverse_edge);		
	}
	
	for(edge_itr=reverse_edges.begin(); edge_itr != reverse_edges.end(); ++edge_itr) {
		edges.push_back(*edge_itr);
	}

	return;
}	

/* 
  make a vector of weighted edges undirected by 
  adding the reveres edge with the same edgeweight
*/
void make_wedgelist_undirected( vector< tuple<int,int,int> >& wedgelist) {


	vector< tuple<int,int,int> >::iterator wedge_itr;
	vector< tuple<int,int,int> > reverse_wedges;
	tuple<int,int,int> wedge, wedge_reverse;
	int v1, v2, weight;
	for( wedge_itr=wedgelist.begin(); wedge_itr != wedgelist.end(); ++wedge_itr) {
		wedge=*wedge_itr;
		v1 = get<0>(wedge);
		v2 = get<1>(wedge);
		weight = get<2>(wedge);
		wedge_reverse = make_tuple(v2,v1,weight);
		reverse_wedges.push_back(wedge_reverse);
	}
	for( wedge_itr=reverse_wedges.begin(); wedge_itr != reverse_wedges.end(); ++wedge_itr) {
		wedgelist.push_back( *wedge_itr );
	}

	cout << reverse_wedges.size() << " edges added, " << wedgelist.size() << " total weighted edges" << endl;
	return;
}

/*
	Print to stdout each edge in an adjacency list
*/
void print_edges( vector<pair<int, vector<int> > >& adjlist) {

	int n1, n2;
	vector<int> row;
	vector<pair<int, vector<int> > >::iterator vitr;
	for(vitr = adjlist.begin(); vitr != adjlist.end(); ++vitr) {
		n1 = (*vitr).first;
		row = (*vitr).second;
		for(int i=0; i<row.size(); i++) {
			n2 = row[i];
			cout << "edge " << n1 << " " << n2 << endl;
		}
	}

	return;
}

/*
	Write an unweighted edgelist to a file as an edge list
*/
void write_unweighted_edgelist( vector< pair<int,int> >& edges, string outfile) {

	ofstream outstream;
	outstream.open(outfile.c_str());
	
	pair<int,int> edge;
	int v1, v2;

	vector< pair<int,int> >::iterator edge_itr;
	for ( edge_itr=edges.begin(); edge_itr != edges.end(); ++edge_itr ) {
		edge=*edge_itr;
		v1=edge.first;
		v2=edge.second;
		outstream << v1 << "\t" << v2 << endl;
	}

	outstream.close();

	return;
}

/*
	Write a weighted edgelist to a file as a weighted edgelist
*/
void write_wedgelist( vector<tuple<int,int,int> >& wedges, string outfile) {

	ofstream outstream;
	outstream.open(outfile.c_str());
	
	tuple<int,int,int> wedge;
	int v1, v2, weight;
	
	vector< tuple<int,int,int> >::iterator wedge_itr;
	for( wedge_itr=wedges.begin(); wedge_itr!=wedges.end(); ++wedge_itr) {
		wedge=*wedge_itr;
		v1 = get<0>(wedge);
		v2 = get<1>(wedge);
		weight = get<2>(wedge);
		outstream << v1 << "\t" << v2 << "\t" << weight << endl;
	}

	outstream.close();

}

