all: example run

CXXFLAGS = -std=c++20

CXXFLAGS := $(CXXFLAGS) -I dijkstra
CXXFLAGS := $(CXXFLAGS) -I generic_dijkstra
CXXFLAGS := $(CXXFLAGS) -I graph
CXXFLAGS := $(CXXFLAGS) -I props
CXXFLAGS := $(CXXFLAGS) -I units

# Use the C++ linker
LINK.o = $(LINK.cc)

.PHONY: run

run: example
	./example

count:
	wc -l *.hpp *.cc

depend:
	g++ -MM $(CXXFLAGS) *.cc > dependencies

include dependencies
