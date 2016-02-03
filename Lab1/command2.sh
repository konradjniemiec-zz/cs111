#!/usr/local/cs/bin/bash
head -c 500 /dev/urandom > a
touch asdf
touch afsd
#c=8
printf "Bash Results:\n"
time cat a | tr ' ' '\n' | \
#while [ $c -gt 0]
#do
#    echo $c
#    sort asdf > afsd
#    shuf afsd > asdf
#    let c=($c-1)
#done
sort > afsd | \
shuf afsd > asdf | \
sort asdf > afsd | \
shuf afsd > asdf | \
sort asdf > afsd | \
shuf afsd > asdf | \
cat afsd


printf "SimpSh Results:\n"

time ./simpsh --rdonly a --rdwr asdf --rdwr afsd --pipe --pipe --pipe --pipe --pipe --pipe --pipe --pipe --creat --wronly temp --command 0 4 4 cat a --command 3 6 6 tr ' ' '\n' --command 5 8 8 sort --command 7 10 10 shuf afsd --command 9 12 12 sort asdf --command 11 14 14 shuf afsd --command 13 16 16 sort asdf --command 15 18 18 shuf afsd --command 17 19 19 cat --wait

# 0 1 2 3-4 5-6 7-8 9-10 11-12 13-14 15-16 17-18

printf "Exceline Results:\n"


rm a
rm afsd
rm asdf
