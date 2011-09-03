CC = gcc
CFLAGS = -Wall -O2

objects = repo.o config.o common.o sync.o update.o

repo : $(objects)
	$(CC) -o repo $(objects)

config.o : config.c config.h
	$(CC) $(CFLAGS) -c config.c
common.o : common.c common.h
	$(CC) $(CFLAGS) -c common.c
sync.o : sync.c common.h
	$(CC) $(CFLAGS) -c sync.c
update.o : update.c common.h
	$(CC) $(CFLAGS) -c update.c
repo.o : repo.c common.h config.h
	$(CC) $(CFLAGS) -c repo.c

clean :
	rm repo $(objects)
