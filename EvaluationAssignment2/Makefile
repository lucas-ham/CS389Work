CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -pedantic -o0 -g -O3 -std=c++20
all: barriers

single: timing.cc timing.hh
	$(CXX) $(CXXFLAGS) -o timing.cc

barriers: barriers.cc barriers.hh
	$(CXX) $(CXXFLAGS) -o single barriers.cc

clean:
	rm -rf *.o *.dSYM timing barriers
