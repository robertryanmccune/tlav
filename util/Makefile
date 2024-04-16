#	Think-Like-A-Vertex Framework, Makefile for Edgelist Preprocessing
#	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>
#
#	Master Makefile
#
#	This software is distributed under 
#	the modified Berkeley Software Distribution (BSD) License.

all: edge_preproc

edge_preproc: 
	g++ -std=c++11 edgelist_preprocessing.cpp -o edge_preproc
	
clean:
	@[ -f edge_preproc ] && rm edge_preproc	 || true