CC=gcc
CFLAGS=

simpsh : main.o
	$(CC) -o simpsh
main.o : main.c
	$(CC) $(CFLAGS) 

