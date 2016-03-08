#!/bin/sh
for i in `seq 1 100`
do
    ./ioctl_crash 5
    ln -s hello.txt test/sym_link
done
./ioctl_crash -1
echo "lol" > test/test_inodes