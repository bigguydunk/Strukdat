CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -g -fdiagnostics-color=always

SRC = src/main.cpp \
	src/menu.cpp \
	src/management/polis.cpp \
	src/management/polis_io.cpp \
	src/management/xorcipher.cpp \
	src/management/encoding.cpp \
	strukdat/simple_queue.cpp

TARGET = main.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	del /f main.exe

.PHONY: all clean
