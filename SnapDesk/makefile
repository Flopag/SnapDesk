CXX = g++
CXXFLAGS = -I ./includes
LDFLAGS = -lssl -lcrypto

SRC = $(filter-out src/test.cpp, $(wildcard src/**/*.cpp src/*.cpp))
OBJ = $(SRC:.cpp=.o)
TARGET = snapdesk

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test:
	g++ ./src/test.cpp -o test

test-run:
	rm ./test
	make test
	./test

run:
	make
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

force:
	make clean
	make