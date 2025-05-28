# Simple Makefile for Strukdat Project (Windows/Unix)
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -g -fdiagnostics-color=always
SRC = src/main.cpp src/menu.cpp src/management/polis.cpp src/management/polis_io.cpp src/management/xorcipher.cpp
TARGET = main.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o src/$(TARGET)

clean:
	rm -f src/$(TARGET)

.PHONY: all clean
