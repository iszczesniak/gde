# CXXFLAGS += -O3
# CXXFLAGS += -O3 -D NDEBUG

CXXFLAGS += -std=c++23
CXXFLAGS += -I .
CXXFLAGS += -I dijkstra
CXXFLAGS += -I generic_dijkstra
CXXFLAGS += -I graph
CXXFLAGS += -I props
CXXFLAGS += -I units
CXXFLAGS += -I /usr/local/include

LDLIBS += -L /usr/local/lib
LDLIBS += -l boost_graph
LDLIBS += -l boost_program_options

# Use the C++ linker
LINK.o = $(LINK.cc)

all: gde
