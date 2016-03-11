

CC=gcc
options=-pthread -lrt -Q -g

all	:	addtest sltest

addtest	:	addtest.c 
	$(CC) -o $@ $^ $(options)

sltest	:	sltest.c SortedList.h
	$(CC) -o $@ $^ $(options)

clean:
	rm -rf *.o addtest
distr:
	tar -cvzf konrad-lab4.tar.gz *.c SortedList.h answers.txt graphs.pdf Makefile