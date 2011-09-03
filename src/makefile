CC = gcc
CFLAGS = -Wall -O2

objects = repo.o config.o sync.o update.o

repo : $(objects)
	$(CC) -o repo $(objects)

repo.o : repo.c common.h config.h
	$(CC) $(CFLAGS) -c repo.c
config.o : config.c config.h
	$(CC) $(CFLAGS) -c config.c
sync.o : sync.c common.h
	$(CC) $(CFLAGS) -c sync.c
update.o : update.c common.h
	$(CC) $(CFLAGS) -c update.c

clean :
	rm repo repo.o config.o sync.o update.o
