PROG = gde
SRCS = gde.cc
MAN =

CXX = clang++-19

# CXXFLAGS += -O3
# CXXFLAGS += -O3 -D NDEBUG

CXXFLAGS += -std=c++2c
CXXFLAGS += -I .
CXXFLAGS += -I dijkstra
CXXFLAGS += -I generic_dijkstra
CXXFLAGS += -I graph
CXXFLAGS += -I props
CXXFLAGS += -I units
CXXFLAGS += -I /usr/local/include

LDFLAGS += -L /usr/local/lib
LDFLAGS += -l boost_graph
LDFLAGS += -l boost_program_options

.include <bsd.prog.mk>
