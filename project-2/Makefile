CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wshadow -ansi -pedantic

all: project2

run:
	./cat1

clean:
	rm -rf cat1 cat1.dSYM

project2: cat1.c
	$(CXX) $(CXXFLAGS) cat1.c -o cat1
