# Simple Makefile for Strukdat Project (Windows/Unix)
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -g -fdiagnostics-color=always
SRC = src/main.cpp src/menu.cpp src/management/polis.cpp src/management/polis_io.cpp
TARGET = main.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o src/$(TARGET)

clean:
	del /q src\*.exe 2>NUL || rm -f src/*.exe

.PHONY: all clean
