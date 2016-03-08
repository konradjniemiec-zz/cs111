#!/bin/sh

./ioctl_crash 3
ln test/world.txt test/to_remove
./ioctl_crash -1