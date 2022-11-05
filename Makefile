CXXFLAGS = -std=c++20

CXXFLAGS := $(CXXFLAGS) -I dijkstra
CXXFLAGS := $(CXXFLAGS) -I generic_dijkstra
CXXFLAGS := $(CXXFLAGS) -I graph
CXXFLAGS := $(CXXFLAGS) -I units

all: example
