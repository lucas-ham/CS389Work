CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -pedantic -o0 -g -O3 -std=c++20
all: make_main

make_main: main.o
	$(CXX) $(CXXFLAGS) -o main main.o

main.o: main.cc main.hh
	$(CXX) $(CXXFLAGS) -c -o main.o main.cc

clean:
	rm -rf *.o main
