CC=gcc
CFLAGS=-O2 -g -std=gnu11

simpsh : main.o
	$(CC) main.o $(CFLAGS) -o simpsh -I.
main.o : main.c
	$(CC) $(CFLAGS) -c main.c -o main.o
dist:

check:
	./test.sh
clean:
	rm -f *.o simpsh *.tar.gz
