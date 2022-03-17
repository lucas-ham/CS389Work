CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -pedantic -o0 -g -O3 -std=c++20
all: threaded

single: singleThreaded.cc main.hh
	$(CXX) $(CXXFLAGS) -o single singleThreaded.cc

threaded: multiThreaded.cc main.hh
	$(CXX) $(CXXFLAGS) -o single multiThreaded.cc

clean:
	rm -rf *.o *.dSYM single threaded 
