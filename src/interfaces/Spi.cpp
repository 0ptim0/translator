#include "Spi.hpp"

#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

Spi::Spi() {}

Spi::~Spi() {
    if (this->fd > 0) {
        close(this->fd);
    }
}

int Spi::init() {
    strncpy(this->path, SPI_DEFAULT_PATH, SPI_MAX_PATH_LENGTH - 1);
    this->fd = open(this->path, O_RDWR);
    if (this->fd < 0) {
        return -1;
    }
    pthread_t thread = {0};
    pthread_attr_t attr = {0};
    pthread_attr_init(&attr);
    pthread_create(&thread, &attr, this->handler, this);
    return 0;
}

int Spi::exec(char *cmd) {
    // Parse binary/string commands
    return 0;
}

void *Spi::handler(void *arg) {
    bool loop = true;
    while (loop) {
        sleep(1);
    }
    return nullptr;
}
