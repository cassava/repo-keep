# Macros

CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS =

# If we have debug or release mode
ifdef DEBUG
	CFLAGS += -ggdb
else
	CFLAGS += -O2
endif

objects = common.o sync.o update.o bm_config.o bm_util.o bm_string.o bm_list.o

.PHONY: clean

######################################################################
repo : $(objects) repo.c common.h
	$(CC) $(CFLAGS) -o repo repo.c $(objects)

common.o : common.c common.h
	$(CC) $(CFLAGS) -c common.c
sync.o : sync.c common.h
	$(CC) $(CFLAGS) -c sync.c
update.o : update.c common.h
	$(CC) $(CFLAGS) -c update.c
bm_string.o : bm_string.c bm_string.h
	$(CC) $(CFLAGS) -c bm_string.c
bm_list.o : bm_list.c bm_list.h
	$(CC) $(CFLAGS) -c bm_list.c
bm_util.o : bm_util.c bm_util.h
	$(CC) $(CFLAGS) -c bm_util.c
bm_config.o : bm_config.c bm_config.h
	$(CC) $(CFLAGS) -c bm_config.c


clean :
	rm repo $(objects)
