CXX = g++
CXXFLAGS = -Iinclude -Wall -std=c++17
LDFLAGS = -lutf8proc

# sources
SRC_CPP = src/tokenizer.cpp src/tfidf.cpp
SRC_MAIN = src/main.cpp

OBJ_CPP = $(SRC_CPP:.cpp=.o)
OBJ_MAIN = $(SRC_MAIN:.cpp=.o)

.PHONY: all clean

all: main

main: $(OBJ_MAIN) $(OBJ_CPP)
								$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean: rm -rf src/*.o main
