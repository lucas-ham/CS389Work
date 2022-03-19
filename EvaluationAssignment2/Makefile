CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -pthread -g -std=c++20
all: barriers_time

timing: timing.cc timing.hh
	$(CXX) $(CXXFLAGS) -o timing timing.cc

barriers: barriers.cc barriers.hh
	$(CXX) $(CXXFLAGS) -o barriers barriers.cc

barriers_test: barriers.cc barriers.hh test_barriers.cc
	$(CXX) $(CXXFLAGS) -o barriers_test test_barriers.cc

barriers_time: barriers.cc barriers.hh generate_barriers.cc
	$(CXX) $(CXXFLAGS) -o barriers_time generate_barriers.cc

clean:
	rm -rf *.o *.dSYM timing barriers barriers_test barriers_time
