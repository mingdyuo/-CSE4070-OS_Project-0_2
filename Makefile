CC = gcc
CFLAGS = -Wall
TARGET = testlib

$(TARGET) : list.o bitmap.o debug.o hash.o hex_dump.o main.o common.o func_list.o func_hash.o func_bitmap.o
	$(CC) $(CFLAGS) -o $(TARGET) list.o bitmap.o debug.o hash.o -lm hex_dump.o main.o common.o func_list.o func_hash.o func_bitmap.o

main.o : main.c common.h
	$(CC) $(CFLAGS) -c -o main.o main.c

common.o : common.c common.h list.h hash.h bitmap.h
	$(CC) $(CFLAGS) -c -o common.o common.c

list.o : list.c list.h
	$(CC) $(CFLAGS) -c -o list.o list.c

func_list.o : func_list.c func_list.h list.h
	$(CC) $(CFLAGS) -c -o func_list.o func_list.c

bitmap.o : bitmap.c bitmap.h
	$(CC) $(CFLAGS) -c -o bitmap.o bitmap.c

func_bitmap.o : func_bitmap.c func_bitmap.h bitmap.h 
	$(CC) $(CFLAGS) -c -o func_bitmap.o func_bitmap.c

debug.o : debug.c debug.h
	$(CC) $(CFLAGS) -c -o debug.o debug.c

hash.o : hash.c hash.h
	$(CC) $(CFLAGS) -c -o hash.o hash.c

func_hash.o : func_hash.c func_hash.h hash.h
	$(CC) $(CFLAGS) -c -o func_hash.o func_hash.c

hex_dump.o : hex_dump.c hex_dump.h
	$(CC) $(CFLAGS) -c -o hex_dump.o hex_dump.c


clean :
	rm *.o $(TARGET)
