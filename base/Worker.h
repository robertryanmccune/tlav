/*
	Think-Like-A-Vertex Framework, Worker.h
	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>

	General Worker class, header and implementation,
	where most of the computation occurs	

	This software is distributed under 
	the modified Berkeley Software Distribution (BSD) License.
*/


#ifndef WORKER_H
#define WORKER_H

#include "gpregellib.h"

using namespace std;

// templated Worker class
template<typename T>
class Worker{

	// constructor and public member functions
	public:
		Worker(string);
		void run();
		// Two virtual functions
		virtual void init_args(int, ...) = 0;
		virtual void write_output(string) = 0;
		void load_graph();

	// private member functions
	private:
		void write_messages();
		void process_inbox();
		void deliver_messages();

		void init_verts();
		void init_msg_queues();

		void log_superstep(int);
		string set_logfilename(string);

	// protected member functions
	protected:
		int* get_csr1();
		void set_csr1(int*);

		int* get_csr2();
		void set_csr2(int*);

		int* get_weights();
		void set_weights(int*);

		int get_num_verts();
		void set_num_verts(int);

		int get_num_edges();
		void set_num_edges(int);

		bool get_logging();
		void set_logging(bool);

		T* get_vert_cont();
		void set_vert_cont(T*);

		Message<int>* get_out_msgs();
		void set_out_msgs(Message<int>*);

		Message<int>* get_in_msgs();
		void set_in_msgs(Message<int>*);

		int get_num_msgs();
		void set_num_msgs(int);

		bool get_fixed_supersteps();
		void set_fixed_supersteps(bool);

	// private data
	private:
		// graph data
		Graph g;
		int* csr1;
		int* csr2;
		int* weights;
		int num_verts;
		int num_edges;

		// logging and writing i/o
		bool logging;
		string logfilename;
		string graphfile;
		string outfile;

		// collection of the vertices, a vertex container
		T* vert_cont;
		
		// in and output message queues
		Message<int>* out_msgs_q;
		Message<int>* in_msgs_q;
		int num_messages;

		int src;
		bool fixed_supersteps;
		int max_supersteps;

};

/*
	Worker constructor function with graph file
	set logging default to true, and create
	log filename with graph file.
	Load the graph and initialize the message queues
	and the opening states of vertices
*/
template<typename T>
Worker<T>::Worker(string infile) {
	graphfile = infile;
	//set_logging(false);
	set_logging(true);
	logfilename = set_logfilename(infile);
	load_graph();
	//init();
	init_msg_queues();
	init_verts();
}

/*
	initialize the input and output message queues
*/
template<typename T>
void Worker<T>::init_msg_queues() {

	// allocate message queues
	out_msgs_q = new Message<int>[ num_edges ];
	in_msgs_q = new Message<int>[ num_edges ];

	// set every message index to a null message
	Message<int> null_msg;
	null_msg.set_dest_id(-1);
	null_msg.set_value(0);

	for(int i=0; i<num_edges; i++) {
		in_msgs_q[i] = null_msg;
		out_msgs_q[i] = null_msg;
	}
	num_messages = 0;
	return;
}

/*
	Initialize the states of each vertex
*/
template<typename T>
void Worker<T>::init_verts() {

	vert_cont = new T[num_verts];

	// Init vertices data with id, neighbors, weights, degree, and
	// the init() function specified by templating (where active verts specified)
	T* vert;
	int degree, offset;
	for(int i=0; i<num_verts; i++) {
		offset = csr1[i];
		degree = csr1[i+1] - offset;

		vert = &vert_cont[i];
		(*vert).set_id(i);		
		(*vert).set_neighborstart( &csr2[offset] );
		(*vert).set_weightstart( &weights[offset] );
		(*vert).set_degree(degree);

		//(*vert).init();
		(*vert).set_value(0);
		(*vert).set_active(false);
	}

	return;
}

/*
	Load the graph data
	graph is assumed to be in edgelist format
	if edge weight is omitted, set weight equal to 1
*/
template<typename T>
void Worker<T>::load_graph() {
	
	g.init_from_edgelist(graphfile);
	csr1 = g.get_csr1();
	csr2 = g.get_csr2();
	weights = g.get_weights();
	num_verts = g.get_num_verts();
	num_edges = g.get_num_dir_edges();

	printf("loaded %d nodes and %d edges\n", num_verts, num_edges);

}

/*
	run the system by:
		- writing messages for the initially activated verts, log, then
		- deliver messages, process inbox/run the vertex program, then write messages
*/
template<typename T>
void Worker<T>::run() {

	printf("executing\n");

	int superstep=0;
	
	write_messages();
	superstep++;
	if( logging ) log_superstep(superstep);

	//while( num_messages > 0 && !(fixed_supersteps && superstep < max_supersteps) ) {
	while(num_messages > 0 ) {
		deliver_messages();
		process_inbox();
		write_messages();
		cout << "completing superstep " << superstep << endl;
		superstep++;
		if( logging ) log_superstep(superstep);
	}
	cout << "execution complete" << endl;

	return;
}

/*
	iterate through all vertices
	if vert is active, then for each neighbor,
	write a message to the outbox.
	outbox is a stack, new messages written to front	
*/
template<typename T>
void Worker<T>::write_messages() {

	T* vert;
	Message<int> msg;
	int neighbor, degree, weight;
	for(int i=0; i<num_verts; i++) {
		vert = &vert_cont[i];
		if( (*vert).get_active() ) {
			degree = (*vert).get_degree();
			//cout << "vert " << (*vert).get_id() << " has degree " << degree << endl;
			for(int j=0; j<degree; j++) {
				neighbor = (*vert).get_neighborstart()[j];
				weight = (*vert).get_weightstart()[j];
				(*vert).send_message( out_msgs_q, num_messages, neighbor, weight );
				num_messages++;
			}
			(*vert).set_active(false);
		}
	}
	//cout << "wrote " << num_messages << " messages" << endl;

	return;
}

/*
	move messages from outqueue to inqueue.
*/
template<typename T>
void Worker<T>::deliver_messages() {

	for(int i=0; i<num_messages; i++) {
		in_msgs_q[i] = out_msgs_q[i];
	}

	return;
}

/*
	vertices read messages from the inbox.
	iterate through inbox, get vert id,
	execute vert, set active as necessary
*/
template<typename T>
void Worker<T>::process_inbox() {

	T* vert;
	Message<int>* msg;

	int value;
	for(int i=0; i<num_messages; i++) {
		msg = &in_msgs_q[i];
		vert = &vert_cont[ (*msg).get_dest_id() ];
		value = vert->get_value();
		//printf("before processing msg, vert: %d, msg dest: %d, msg val: %d\n", (*vert).get_id(), (*msg).get_dest_id(), (*msg).get_value() );
		vert->process_message(msg);
		//printf("after processing msg %d\n", i);
		if( value != (*vert).get_value() && (*vert).get_active() == false ) {
			vert->set_active(true);
			//cout << "setting active" << endl;
		}		
	}
	num_messages=0;

	return;
}



/** Logging functions **/
template<typename T>
void Worker<T>::log_superstep(int superstep) {

	ofstream logfile;
	logfile.open( logfilename.c_str() , ios::app );

	logfile << "/***********  superstep " << superstep << " **********/" << endl;
	logfile << "vert" << endl;

	T vert;
	int id, val;
	string inf = "inf";
	for(int i=0; i<num_verts; i++) {

		vert = vert_cont[i];
		id = vert.get_id();
		val = vert.get_value();
		
		logfile << id << "\t";
		if( val == INT_MAX) 
			logfile << inf << endl;
		else
			logfile << val << endl;

	}

	logfile << endl << "messages_out" << endl;
	logfile << "num\tdest_id\tvalue" << endl;
	
	for(int i=0; i<num_messages; i++) {
		logfile << i << "\t" << out_msgs_q[i].get_dest_id() << "\t" << out_msgs_q[i].get_value() << endl;
	}
	logfile << endl;

	logfile.close();

	return;
}

/*
	set the graph file name based on 
	the graphfile name and time of logging
*/
template<typename T>
string Worker<T>::set_logfilename(string graphstr) {

	string logfilestr = "log/";

	string graphstr_nodirs;
	int pos = graphstr.find_last_of('/');
	graphstr_nodirs = graphstr.substr( pos+1, graphstr.length() - pos -1 );
	
	logfilestr += graphstr_nodirs;
	logfilestr.erase( logfilestr.find_last_of('.') );
	logfilestr+="_";
	
	time_t ttime;
	struct tm *gtime;
	time(&ttime);
	gtime = gmtime(&ttime);

	char buffer[15];
	strftime( buffer, sizeof(buffer), "%Y\%m\%d\%H\%M\%S", gtime);
	logfilestr+=buffer;
	logfilestr+=".log";

	printf("logfile set as: %s\n", logfilestr.c_str() );
	return logfilestr;

}

/** end logging functions **/

/** protected member functions **/

template<typename T>
int* Worker<T>::get_csr1() {
	return csr1;
}

template<typename T>
void Worker<T>::set_csr1(int* _csr1){
	csr1 = _csr1;
}

template<typename T>
int* Worker<T>::get_csr2(){
	return csr2;
}

template<typename T>
void Worker<T>::set_csr2(int* _csr2) {
	csr2 = _csr2;
}

template<typename T>
int* Worker<T>::get_weights(){
	return weights;
}

template<typename T>
void Worker<T>::set_weights(int* w){
	weights = w;
}

template<typename T>
int Worker<T>::get_num_verts(){
	return num_verts;
}

template<typename T>
void Worker<T>::set_num_verts(int vs){
	num_verts = vs;
}

template<typename T>
int Worker<T>::get_num_edges(){
	return num_edges;
}

template<typename T>
void Worker<T>::set_num_edges(int es){
	num_edges = es;
}

template<typename T>
bool Worker<T>::get_logging(){
	return logging;
}

template<typename T>
void Worker<T>::set_logging(bool log){
	logging = log;
}

template<typename T>
T* Worker<T>::get_vert_cont() {
	return vert_cont;
}

template<typename T>
void Worker<T>::set_vert_cont(T* vc){
	vert_cont = vc;
}

template<typename T>
Message<int>* Worker<T>::get_out_msgs(){
	return out_msgs_q;
}

template<typename T>
void Worker<T>::set_out_msgs(Message<int>* omq){
	out_msgs_q = omq;
}

template<typename T>
Message<int>* Worker<T>::get_in_msgs(){
	return in_msgs_q;
}

template<typename T>
void Worker<T>::set_in_msgs(Message<int>* imq){
	in_msgs_q = imq;
}

template<typename T>
int Worker<T>::get_num_msgs(){
	return num_messages;
}

template<typename T>
void Worker<T>::set_num_msgs(int nm){
	num_messages = nm;
}

/* 
	future work

template<typename T>
bool Worker<T>::get_fixed_supersteps() {
	return fixed_supersteps;
}

template<typename T>
void Worker<T>::set_fixed_supersteps(bool fxss) {
	fixed_supersteps = fxss;
}
*/

/** End protected member function **/


#endif
