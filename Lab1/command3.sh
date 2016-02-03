#!/usr/local/cs/bin/bash

head -c 100000 > b

printf "Bash Results:\n"
cat b | tr ' ' '\n' | sort | grep "abc"

printf "SimpSh Results:\n"

printf "Exceline Results:\n"

rm b
