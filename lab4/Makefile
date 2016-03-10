

CC=gcc
options=-pthread -lrt -Q -g

all	:	addtest sltest

addtest	:	addtest.c 
	$(CC) -o $@ $^ $(options)

sltest	:	sltest.c SortedList.h
	$(CC) -o $@ $^ $(options)

clean:
	rm -rf *.o addtest
