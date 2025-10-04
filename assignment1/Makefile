CC = gcc
CFLAGS = -O3 -fopenmp -Wall
TARGET = conv_test

all: $(TARGET)

$(TARGET): conv.o
	$(CC) $(CFLAGS) -o $(TARGET) conv.o

conv.o: conv.c
	$(CC) $(CFLAGS) -c conv.c

clean:
	rm -f *.o $(TARGET)
