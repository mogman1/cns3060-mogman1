CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wshadow -ansi -pedantic -pthread

all: project

parta: parta.c
	$(CXX) $(CXXFLAGS) parta.c -o parta

partb: partb.c
	$(CXX) $(CXXFLAGS) partb.c -o partb

partc: partc.c
	$(CXX) $(CXXFLAGS) partc.c -o partc

partd: partd.c
	$(CXX) $(CXXFLAGS) partd.c -o partd

clean:
	rm -rf parta partb partc partd parta.dSYM partb.dSYM partc.dSYM partd.dSYM

project: parta partb partc partd
