CC = gcc
CFLAGS = -Wall -O2

objects = config_bm.o util_bm.o

libbm.a : $(objects)
	ar rcs libbm.a $(objects)

config_bm.o : config_bm.c config_bm.h
	$(CC) $(CFLAGS) -c config_bm.c
util_bm.o : util_bm.c util_bm.h
	$(CC) $(CFLAGS) -c util_bm.c

clean :
	rm libbm.a $(objects)
