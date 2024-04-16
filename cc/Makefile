#	Think-Like-A-Vertex Framework, Connected Components Makefile
#	Copyright 2024, Ryan McCune	<robertryanmccune@gmail.com>
#
#	Makefile to compile the BFS programs
#
#	This software is distributed under 
#	the modified Berkeley Software Distribution (BSD) License.

# BASE AND LIBRARY CLASSES
HOME = ..

LIB = $(HOME)/lib

DA = $(LIB)/dynamic_array
DS = $(DA)/DynamicArray.cpp

GR = $(LIB)/Graph
GS = $(GR)/Graph.cpp

BASE = $(HOME)/base

# VERTEX CLASES
VERT_BASE = $(HOME)/base/Vertex.cpp
VERT_DERV = VertexCC.cpp

#MAIN FUNCTION
MAIN = main_cc.cpp

#TARGETS
TARGET = tlav_cc

tlav_cc: $(MAIN)
	g++ -I$(GR) -I$(DA) -I$(HOME) -I$(LIB) -I$(BASE) $(DS) $(GS) $(VERT_BASE) $(VERT_DERV) $(MAIN) -o $(TARGET)

clean:
	@[ -f $(TARGET) ] && rm $(TARGET) || true
