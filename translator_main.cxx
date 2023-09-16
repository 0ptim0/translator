#include <fcntl.h>
#include <unistd.h>

#include <cstdio>
#include <nuttx/can/can.h>

extern "C" int main(int argc, char **argv) {
    int fd = open("/dev/ttyS0", O_RDWR);
    char a[] = "Translator\n";
    write(fd, a, sizeof(a));
    close(fd);
    return 0;
}
