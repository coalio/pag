# the compiler: gcc for C program, define as g++ for C++
CC = g++
CFLAGS  = -g -Wall
TARGET = pag
INSTALL_DIR = /usr/local/bin
INSTALL = install -m 755 -s

all: $(TARGET)
$(TARGET): main.cpp
						$(CC) $(CFLAGS) -o $(TARGET) main.cpp

clean:
						$(RM) $(TARGET)

# install target
install:
						$(INSTALL) $(TARGET) $(INSTALL_DIR)