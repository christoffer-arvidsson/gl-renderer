CXX = gcc
CXXFLAGS = -Wall -Wextra -std=c17 -pedantic -ggdb
LIBS = -lglfw -lGLEW -lGL -lX11 -lopenal -lm -pthread -ldl 
SRC_DIR = src/
INCLUDE = -I include/
TARGET = bin/main

# Get a list of all .cpp files in the src/ directory
SRC := $(wildcard $(SRC_DIR)*.c)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(INCLUDE) $(LIBS)

run: $(TARGET)
	$(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)
