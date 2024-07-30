CXX = gcc
CXXFLAGS = -Wall -Wextra -std=c17 -pedantic -ggdb
LIBS = -lglfw -lGLEW -lGL -lX11 -lopenal -lm -pthread -ldl
SRC_DIR = src/
TARGET = bin/main

# Get a list of all .cpp files in the src/ directory
# SRC := $(wildcard $(SRC_DIR)*.c)
SRC := $(shell find $(SRC_DIR) -type f -name '*.c')

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LIBS)

run: $(TARGET)
	$(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)
