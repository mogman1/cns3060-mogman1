CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wshadow -ansi -pedantic
PROJECT_NAME = simulation

all: project

run:
	./$(PROJECT_NAME)

clean:
	rm -rf $(PROJECT_NAME) $(PROJECT_NAME).dSYM

project: $(PROJECT_NAME).c
	$(CXX) $(CXXFLAGS) $(PROJECT_NAME).c -o $(PROJECT_NAME)
