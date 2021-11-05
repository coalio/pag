# the compiler: gcc for C program, define as g++ for C++
CC = g++
CFLAGS  = -g -Wall
TARGET = pag

all: $(TARGET)
$(TARGET): main.cpp
						$(CC) $(CFLAGS) -o $(TARGET) main.cpp

clean:
						$(RM) $(TARGET)