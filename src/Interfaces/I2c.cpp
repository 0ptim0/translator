#include "I2c.hpp"

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

i2c::Interface::Interface(const char *name, const char *path,
                          interface::Mode mode)
    : Base(name, path, mode) {
}

i2c::Interface::~Interface() {
}

int i2c::Interface::init() {
    int perm = this->m_mode == interface::READ_ONLY    ? O_RDONLY
               : this->m_mode == interface::WRITE_ONLY ? O_WRONLY
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

int i2c::Interface::exec(const char *cmd) {
    return 0;
}

ssize_t i2c::Interface::read(void *data, size_t size) {
    memcpy(this->tx_trans_buf, data, size);
    i2c_msg_s msg[2] = {{.frequency = this->m_frequency,
                         .addr = this->m_address,
                         // TODO: set flags via config
                         .flags = I2C_M_NOSTOP,
                         .buffer = this->tx_trans_buf,
                         .length = static_cast<uint8_t>(size)},
                        {.frequency = this->m_frequency,
                         .addr = this->m_address,
                         // TODO: set flags via config
                         .flags = I2C_M_READ,
                         .buffer = reinterpret_cast<uint8_t *>(data),
                         .length = static_cast<uint8_t>(size)}};
    i2c_transfer_s trans = {
        .msgv = msg,
        .msgc = 2,
    };
    int rv = ioctl(this->fd, I2CIOC_TRANSFER, &trans);
    if (rv) {
        return -1;
    }
    return size;
}

ssize_t i2c::Interface::write(const void *data, size_t size) {
    memcpy(this->tx_trans_buf, data, size);
    i2c_msg_s msg[2] = {{.frequency = this->m_frequency,
                         .addr = this->m_address,
                         // TODO: set flags via config
                         .flags = I2C_M_NOSTOP,
                         .buffer = this->tx_trans_buf,
                         .length = static_cast<uint8_t>(size)},
                        {.frequency = this->m_frequency,
                         .addr = this->m_address,
                         // TODO: set flags via config
                         .flags = I2C_M_READ,
                         .buffer = this->rx_trans_buf,
                         .length = static_cast<uint8_t>(size)}};
    i2c_transfer_s trans = {
        .msgv = msg,
        .msgc = 2,
    };
    int rv = ioctl(this->fd, I2CIOC_TRANSFER, &trans);
    if (rv) {
        return -1;
    }
    return size;
}
