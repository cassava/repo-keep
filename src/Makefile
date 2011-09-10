CC = gcc
CFLAGS = -Wall -O2

objects = common.o sync.o update.o bm_config.o bm_util.o

.PHONY: default clean

######################################################################
repo : $(objects) repo.c common.h
	$(CC) $(CFLAGS) -o repo repo.c $(objects)

common.o : common.c common.h
	$(CC) $(CFLAGS) -c common.c
sync.o : sync.c common.h
	$(CC) $(CFLAGS) -c sync.c
update.o : update.c common.h
	$(CC) $(CFLAGS) -c update.c
bm_util.o : bm_util.c bm_util.h
	$(CC) $(CFLAGS) -c bm_util.c
bm_config.o : bm_config.c bm_config.h
	$(CC) $(CFLAGS) -c bm_config.c


clean :
	rm repo $(objects)
