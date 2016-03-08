#!/bin/sh

echo "lol" > test/new
./ioctl_crash 5
rm test/new
./ioctl_crash -1
echo "hi" > test/new_file