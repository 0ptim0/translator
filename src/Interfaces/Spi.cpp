#include "Spi.hpp"

#include <errno.h>
#include <fcntl.h>
#include <nuttx/config.h>
#include <nuttx/spi/spi.h>
#include <nuttx/spi/spi_transfer.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <syslog.h>
#include <unistd.h>

#include "Message.hpp"

using namespace interface;

Spi::Spi(const char *name, const char *path, interface::Mode mode)
    : InterfaceBase(name, path, mode) {
}

Spi::~Spi() {
}

int Spi::init() {
    int perm = this->m_mode == READ_ONLY    ? O_RDONLY
               : this->m_mode == WRITE_ONLY ? O_WRONLY
                                            : O_RDWR;
    this->fd = open(this->m_path, perm);
    if (this->fd < 0) {
        syslog(LOG_ERR, "Failed to open %s in %s", this->m_name, this->m_path);
        syslog(LOG_ERR, "%s", strerror(errno));
        return -1;
    }

    sprintf(this->m_queue, "/mq_%s", this->m_name);
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 1024;
    attr.mq_msgsize = sizeof(Message);
    attr.mq_curmsgs = 0;
    this->src = mq_open(this->m_queue, O_RDWR | O_CREAT, 0666, &attr);
    if (this->src < 0) {
        syslog(LOG_ERR, "Failed to create queue for %s", this->m_name);
        syslog(LOG_ERR, "%s", strerror(errno));
        close(this->fd);
        return -1;
    }

    this->run();
    return 0;
}

int Spi::exec(const char *cmd) {
    return 0;
}

ssize_t Spi::read(void *data, size_t size) {
    char tx[] = {'a'};
    spi_trans_s trans = {
        .nwords = 1,
        .txbuffer = tx,
        .rxbuffer = data,
    };
    spi_sequence_s seq = {.mode = SPIDEV_MODE0,
                          .nbits = 8,
                          .ntrans = 1,
                          .frequency = 100000,
                          .trans = &trans};
    return ioctl(this->fd, SPIIOC_TRANSFER, &seq);
}

ssize_t Spi::write(const void *data, size_t size) {
    char rx[1024];
    spi_trans_s trans = {
        .nwords = size,
        .txbuffer = data,
        .rxbuffer = rx,
    };
    spi_sequence_s seq = {.ntrans = 1, .frequency = 100, .trans = &trans};
    int rv = ioctl(this->fd, SPIIOC_TRANSFER, &seq);
    return !rv ? size : -1;
}
