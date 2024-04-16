#	Think-Like-A-Vertex Framework, Master Makefile
#	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>
#
#	Makefile to compile all programs
#
#	This software is distributed under 
#	the modified Berkeley Software Distribution (BSD) License.

# BASE AND LIBRARY CLASSES
HOME = .

LIB = $(HOME)/lib

BASE = $(HOME)/base

DA = $(LIB)/dynamic_array
DS = $(DA)/DynamicArray.cpp

GR = $(LIB)/Graph
GS = $(GR)/Graph.cpp

VERT_BASE = $(BASE)/Vertex.cpp

# Breadth-First Search
BFS = $(HOME)/bfs
BFS_VERT_DERV = $(BFS)/VertexBFS.cpp
BFS_MAIN = $(BFS)/main_bfs.cpp
BFS_TARGET = tlav_bfs

# CONNECTED COMPONENTS
CC = $(HOME)/cc
CC_VERT_DERV = $(CC)/VertexCC.cpp
CC_MAIN = $(CC)/main_cc.cpp
CC_TARGET = tlav_cc

# SINGLE-SOURCE SHORTEST PATH
SSSP = $(HOME)/sssp
SSSP_VERT_DERV = $(SSSP)/VertexSSSP.cpp
SSSP_MAIN = $(SSSP)/main_sssp.cpp
SSSP_TARGET = tlav_sssp

# EDGELIST PREPROCESSING
UTIL = $(HOME)/util
UTIL_MAIN = $(UTIL)/edgelist_preprocessing.cpp
UTIL_TARGET = edge_preproc

# TARGETS
all: tlav_bfs tlav_cc tlav_sssp edge_preproc

tlav_bfs: $(MAIN)
	g++ -I$(GR) -I$(DA) -I$(LIB) -I$(BASE) $(DS) $(GS) $(VERT_BASE) $(WORKER_BASE) $(BFS_VERT_DERV) $(BFS_MAIN) -o $(BFS)/$(BFS_TARGET)

tlav_cc: $(MAIN)
	g++ -I$(GR) -I$(DA) -I$(LIB) -I$(BASE) $(DS) $(GS) $(VERT_BASE) $(WORKER_BASE) $(CC_VERT_DERV) $(CC_MAIN) -o $(CC)/$(CC_TARGET)
	
tlav_sssp: $(MAIN)
	g++ -I$(GR) -I$(DA) -I$(LIB) -I$(BASE) $(DS) $(GS) $(VERT_BASE)  $(WORKER_BASE) $(SSSP_VERT_DERV) $(SSSP_MAIN) -o $(SSSP)/$(SSSP_TARGET)

edge_preproc: $(MAIN)
	g++ -std=c++11 $(UTIL_MAIN) -o $(UTIL)/$(UTIL_TARGET)

clean:
	@[ -f $(BFS)/$(BFS_TARGET) ] && rm $(BFS)/$(BFS_TARGET) || true
	@[ -f $(CC)/$(CC_TARGET) ] && rm $(CC)/$(CC_TARGET) || true
	@[ -f $(SSSP)/$(SSSP_TARGET) ] && rm $(SSSP)/$(SSSP_TARGET) || true
	@[ -f $(UTIL)/$(UTIL_TARGET) ] && rm $(UTIL)/$(UTIL_TARGET) || true