CXX = g++
CXXFLAGS = -std=c++14 -Iinc -Wall -Wextra -g

TARGET = Lab1
SRC = main.cpp $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

rebuild: all

.PHONY: all rebuild
