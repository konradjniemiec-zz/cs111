#!/usr/local/cs/bin/bash
head -c 500 /dev/urandom > aasdf

printf "Bash Results:\n"
time cat aasdf | tr ' ' '\n' | tr '[a-z]' '[A-Z]' | sort > out

printf "Simpsh Results:\n"

time ./simpsh --rdwr aasdf --pipe --pipe --pipe --trunc --wronly out --command 0 2 2 cat aasdf --command 1 4 4 tr ' ' '\n' --command 3 6 6 tr '[a-z]' '[A-Z]' --command 5 7 7 sort --wait

printf "Exceline Results:\n"

time ./command1_exec.sh

rm out 
rm aasdf
