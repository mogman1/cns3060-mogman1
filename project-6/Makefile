CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wshadow -ansi -pedantic

all: project

parta:
	$(CXX) $(CXXFLAGS) parta.c -o parta

partb:
	$(CXX) $(CXXFLAGS) partb.c -o partb

partc:
	$(CXX) $(CXXFLAGS) partc.c -o partc

partd:
	$(CXX) $(CXXFLAGS) partd.c -o partd

clean:
	rm -rf parta partb partc partd parta.dSYM partb.dSYM partc.dSYM partd.dSYM

project: parta partb partc partd
