#!/usr/local/cs/bin/bash

head -c 100000 /dev/urandom > b

printf "Bash Results:\n"
time cat b | tr ' ' '\n' | sort | grep "abc"

printf "SimpSh Results:\n"

printf "Exceline Results:\n"

rm b
