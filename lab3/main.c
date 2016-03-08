#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <signal.h>
#include "ospfs.h"


int main(int argc, char** argv) {
    int num = atoi(argv[1]);
    int fd = open("test/foo",O_NONBLOCK | O_CREAT);
    if (fd < 0)
      raise (SIGABRT);
    ioctl(fd,OSPFS_CRASH_COUNTER, num);
    close(fd);
}
