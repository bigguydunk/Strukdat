CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Isrc -g -fdiagnostics-color=always

SRC = src/main.cpp \
    src/menu.cpp \
    src/polis/polis.cpp \
    src/polis/polis_io.cpp \
    src/strukdat/xorcipher.cpp \
    src/strukdat/encoding.cpp \
    src/strukdat/strukdat_queue.cpp \
    src/strukdat/polis_list.cpp \
    src/strukdat/strukdat_stack.cpp \
    src/strukdat/polis_list_sort.cpp

TARGET = main.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	del /f main.exe

.PHONY: all clean
