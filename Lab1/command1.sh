#!/usr/local/cs/bin/bash
head -c 500 /dev/urandom > aasdf

printf "Bash Results:\n"
time cat aasdf | tr ' ' '\n' | tr '[a-z]' '[A-Z]' | sort > out

printf "Simpsh Results:\n"


printf "Exceline Results:\n"

rm aasdf
