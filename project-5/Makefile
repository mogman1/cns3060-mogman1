CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wshadow -ansi -pedantic
PROJECT_NAME = more1

all: project

run:
	./$(PROJECT_NAME) test.txt

clean:
	rm -rf $(PROJECT_NAME) $(PROJECT_NAME).dSYM

project: $(PROJECT_NAME).c
	$(CXX) $(CXXFLAGS) $(PROJECT_NAME).c -o $(PROJECT_NAME)
