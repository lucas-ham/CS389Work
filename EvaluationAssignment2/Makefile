CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -pedantic -o0 -g -O3 -std=c++20
all: make_main

make_main: problemOne.o
	$(CXX) $(CXXFLAGS) -o probOne problemOne.o

problemOne.o: problemOne.cc main.hh
	$(CXX) $(CXXFLAGS) -c -o problemOne.o problemOne.cc

clean:
	rm -rf *.o probOne
