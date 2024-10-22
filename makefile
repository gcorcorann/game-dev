# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -g -std=c++17

# Target executable
TARGET = game.exe

# Libraries (using pkg-config)
LIBS = $(shell pkg-config --cflags --libs sfml-all)

# Souce CPP file
#SRC = simple_game.cpp
SRC = moving_background.cpp

build:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LIBS)

run: build
	./game.exe

