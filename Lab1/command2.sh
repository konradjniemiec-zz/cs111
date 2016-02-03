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
cat asdf


printf "SimpSh Results:\n"


printf "Exceline Results:\n"

rm asdf
rm afsd
rm a
