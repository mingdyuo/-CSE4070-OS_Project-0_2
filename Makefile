CC = gcc
CFLAGS = -Wall
TARGET = testlib

$(TARGET) : list.o bitmap.o debug.o hash.o hex_dump.o main.o
	$(CC) $(CFLAGS) -o $(TARGET) list.o bitmap.o debug.o hash.o hex_dump.o main.o

main.o : main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

list.o : list.c list.h
	$(CC) $(CFLAGS) -c -o list.o list.c

bitmap.o : bitmap.c bitmap.h
	$(CC) $(CFLAGS) -c -o bitmap.o bitmap.c

debug.o : debug.c debug.h
	$(CC) $(CFLAGS) -c -o debug.o debug.c

hash.o : hash.c hash.h
	$(CC) $(CFLAGS) -c -o hash.o hash.c

hex_dump.o : hex_dump.c hex_dump.h
	$(CC) $(CFLAGS) -c -o hex_dump.o hex_dump.c


clean :
	rm *.o $(TARGET)
