CC=gcc
CFLAGS=-O2

simpsh : main.o
	$(CC) $(CFLAGS) -o simpsh main.o -I.
main.o : main.c
	$(CC) $(CFLAGS) main.c -o main.o
dist:

check:

clean:
	rm -f *.o simpsh *.tar.gz
