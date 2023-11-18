#include "Uart.hpp"

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <syslog.h>

Uart::Uart() {
}

Uart::~Uart() {
    if (this->fd > 0) {
        close(this->fd);
    }
}

int Uart::init() {
    strncpy(this->path, UART_DEFAULT_PATH, UART_MAX_PATH_LENGTH - 1);
    // this->fd = open(this->path, O_RDWR);
    // if (this->fd < 0) {
    //     return -1;
    // }
    pthread_t thread = {0};
    pthread_attr_t attr = {0};
    pthread_attr_init(&attr);
    pthread_create(&thread, &attr, this->handler, this);
    return 0;
}

int Uart::exec(char *cmd) {
    // Parse binary/string commands
    return 0;
}

void *Uart::handler(void *arg) {
    bool loop = true;
    while (loop) {
        sleep(1);
    }
    return NULL;
}
