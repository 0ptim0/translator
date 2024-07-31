#include "Spi.hpp"

#include <errno.h>
#include <fcntl.h>
#include <nuttx/config.h>
#include <nuttx/spi/spi_transfer.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <syslog.h>
#include <unistd.h>

#include "Message.hpp"

spi::Interface::Interface(const char *name, const char *path,
                          interface::Mode mode)
    : interface::Base(name, path, mode) {
}

spi::Interface::~Interface() {
}

int spi::Interface::init() {
    int perm = this->m_mode == interface::READ_ONLY    ? O_RDONLY
               : this->m_mode == interface::WRITE_ONLY ? O_WRONLY
                                                       : O_RDWR;
    this->fd = open(this->m_path, perm);
    if (this->fd < 0) {
        syslog(LOG_ERR, "Failed to open %s in %s", this->m_name, this->m_path);
        syslog(LOG_ERR, "%s", strerror(errno));
        return -1;
    }

    this->run();
    return 0;
}

ssize_t spi::Interface::read(void *data, size_t size) {
    memcpy(this->tx_trans_buf, data, size);
    spi_trans_s trans = {
        .nwords = size,
        .txbuffer = this->tx_trans_buf,
        .rxbuffer = data,
    };
    spi_sequence_s seq = {.mode = this->m_spi_mode,
                          .nbits = this->m_nbits,
                          .ntrans = this->m_ntrans,
                          .frequency = this->m_frequency,
                          .trans = &trans};
    int rv = ioctl(this->fd, SPIIOC_TRANSFER, &seq);
    if (rv) {
        return -1;
    }
    return size;
}

ssize_t spi::Interface::write(const void *data, size_t size) {
    memcpy(this->tx_trans_buf, data, size);
    spi_trans_s trans = {
        .nwords = size,
        .txbuffer = this->tx_trans_buf,
        .rxbuffer = this->rx_trans_buf,
    };
    spi_sequence_s seq = {.mode = this->m_spi_mode,
                          .nbits = this->m_nbits,
                          .ntrans = this->m_ntrans,
                          .frequency = this->m_frequency,
                          .trans = &trans};
    int rv = ioctl(this->fd, SPIIOC_TRANSFER, &seq);
    if (rv) {
        return -1;
    }
    return size;
}

int spi::Interface::exec(const char *command) {
    syslog(LOG_DEBUG, "%s: command: %s", this->name(), command);
    return 0;
}
