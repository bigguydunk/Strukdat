CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -g -fdiagnostics-color=always

SRC = src/main.cpp \
    src/menu.cpp \
    src/polis/polis.cpp \
    src/polis/polis_io.cpp \
    strukdat/xorcipher.cpp \
    strukdat/encoding.cpp \
    strukdat/strukdat_queue.cpp \
    strukdat/polis_list.cpp \
    strukdat/strukdat_stack.cpp \
    strukdat/polis_list_sort.cpp

TARGET = main.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	del /f main.exe

.PHONY: all clean
