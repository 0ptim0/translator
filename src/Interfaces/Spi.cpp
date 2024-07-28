#include "Spi.hpp"

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include "Message.hpp"

Spi::Spi() {
    strncpy(this->m_name, SPI_DEFAULT_NAME, name_max_length);
    strncpy(this->m_path, SPI_DEFAULT_PATH, path_max_length);
}

Spi::Spi(const char *name, const char *path) {
    if (name != nullptr) {
        strncpy(this->m_name, name, name_max_length);
    }
    if (path != nullptr) {
        strncpy(this->m_path, path, path_max_length);
    }
}

Spi::~Spi() {
    if (this->fd > 0) {
        close(this->fd);
    }
    if (this->src > 0) {
        mq_close(this->src);
    }
    for (int i = 0; i < max_interfaces; ++i) {
        if (this->dst[i] > 0) {
            mq_close(this->dst[i]);
        }
    }
}

int Spi::init() {
    this->fd = open(this->m_path, O_RDWR);
    if (this->fd < 0) {
        syslog(LOG_ERR, "Failed to open %s in %s", this->m_name, this->m_path);
        syslog(LOG_ERR, "%s", strerror(errno));
        return -1;
    }

    sprintf(this->m_queue, "/mq_%s", this->m_name);
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 100;
    attr.mq_msgsize = sizeof(Message);
    attr.mq_curmsgs = 0;
    this->src = mq_open(this->m_queue, O_RDONLY | O_CREAT, 0666, &attr);
    if (this->src < 0) {
        syslog(LOG_ERR, "Failed to create queue for %s", this->m_name);
        syslog(LOG_ERR, "%s", strerror(errno));
        close(this->fd);
        return -1;
    }

    pthread_t tx_pthread = {0};
    pthread_attr_t tx_attr = {0};
    pthread_attr_init(&tx_attr);
    pthread_create(&tx_pthread, &tx_attr, this->threadTx,
                   static_cast<InterfaceBase *>(this));

    pthread_t rx_pthread = {0};
    pthread_attr_t rx_attr = {0};
    pthread_attr_init(&rx_attr);
    pthread_create(&rx_pthread, &rx_attr, this->threadRx,
                   static_cast<InterfaceBase *>(this));
    return 0;
}

int Spi::exec(char *cmd) {
    return 0;
}