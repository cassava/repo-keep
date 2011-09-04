CC = gcc
CFLAGS = -Wall -O2

objects = common.o sync.o update.o

repo : $(objects) common.h
	make -C lib/
	$(CC) $(CFLAGS) -static -Llib repo.c -lbm -o repo $(objects)

common.o : common.c common.h
	$(CC) $(CFLAGS) -c common.c
sync.o : sync.c common.h
	$(CC) $(CFLAGS) -c sync.c
update.o : update.c common.h
	$(CC) $(CFLAGS) -c update.c

clean :
	make -C lib/ clean
	rm repo $(objects)
