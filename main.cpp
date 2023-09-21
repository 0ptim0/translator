#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include <cstdio>

int intr = 1;

void intr_handler(int signo) {
    intr = 0;
}

void *echo_thread(void *arg) {
    int fd = open("/dev/ttyS1", O_RDWR);
    if (fd < 0) {
        printf("Failed to open: %s\n", strerror(errno));
        return NULL;
    }
    struct termios termios;
    tcgetattr(fd, &termios);
    termios.c_lflag &= ~ICANON;
    termios.c_cc[VTIME] = 10;
    tcsetattr(fd, TCSANOW, &termios);
    char buf[32] = {0};
    while (intr) {
        ssize_t rb = read(fd, buf, sizeof(buf));
        if (rb < 0) {
            printf("Failed to read: %s\n", strerror(errno));
            continue;
        }
        ssize_t wb = write(fd, buf, rb);
        if (wb < 0) {
            printf("Failed to write: %s\n", strerror(errno));
        }
    }
    close(fd);
    return NULL;
}

extern "C" int main(int argc, char **argv) {
    signal(SIGINT, &intr_handler);
    signal(SIGKILL, &intr_handler);
    signal(SIGTERM, &intr_handler);
    pthread_t pthread = {0};
    pthread_attr_t attr = {0};
    pthread_attr_init(&attr);
    pthread_create(&pthread, &attr, echo_thread, nullptr);
    pthread_join(pthread, NULL);
    printf("Exit\n");
    intr = 1;
    return 0;
}
