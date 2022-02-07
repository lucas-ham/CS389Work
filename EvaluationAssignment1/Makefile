CXX=gcc
CXXFLAGS=-Wall -Wextra -Werror -pedantic -o0 -g

all: make_main

make_main: update_locations.o
	$(CXX) $(CXXFLAGS) -o update_locations update_locations.o

update_locations.o: update_locations.c main.h
	$(CXX) $(CXXFLAGS) -c -o update_locations.o update_locations.c

clean:
	rm -rf *.o update_locations
