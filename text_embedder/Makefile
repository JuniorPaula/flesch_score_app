CXX = g++
CXXFLAGS = -Iinclude -Wall -std=c++17 -fPIC
LDFLAGS = -lutf8proc

# sources
SRC_CPP = src/tokenizer.cpp src/tfidf.cpp src/text_embedder.cpp
SRC_MAIN = src/main.cpp

OBJ_CPP = $(SRC_CPP:.cpp=.o)
OBJ_MAIN = $(SRC_MAIN:.cpp=.o)

# shared lib name
LIB_NAME = libtext_embedder.so

.PHONY: all clean

all: main $(LIB_NAME)

# compile main bin
main: $(OBJ_MAIN) $(OBJ_CPP)
								$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# compile shared lib
$(LIB_NAME): $(OBJ_CPP)
								$(CXX) -shared -o $@ $^ $(LDFLAGS)

# rule to compile files .cpp in .o using CXXFLAGS
%.o: %.cpp
		$(CXX) $(CXXFLAGS) -c $< -o $@

clean: rm -rf src/*.o main
