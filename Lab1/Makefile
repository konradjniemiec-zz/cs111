CC=gcc
CFLAGS=-O2 -g -std=c11
USER=konradjniemiec


simpsh : main.o
	$(CC) main.o $(CFLAGS) -o simpsh -I.
main.o : main.c
	$(CC) $(CFLAGS) -c main.c -o main.o
dist: lab1-$(USER).tar.gz

lab1-$(USER).tar.gz : main.c simpsh Makefile README.md
	make clean
	tar -cvf $@ * 

check:
	./test.sh
clean:
	rm -f *.o simpsh *.tar.gz *~
