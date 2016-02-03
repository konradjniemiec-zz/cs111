#!/usr/local/cs/bin/bash

head -c 100000 /dev/urandom > b

printf "Bash Results:\n"
time cat b | tr ' ' '\n' | sort | grep "abc"

printf "SimpSh Results:\n"

time ./simpsh --rdonly b --pipe --pipe --pipe --creat --wronly temp --command 0 2 2 cat b --command 1 4 4 tr ' ' '\n' --command 3 6 6 sort --command 5 7 7 grep "abc" --wait

printf "Exceline Results:\n"

time ./command3_exec.sh

rm b
rm temp
