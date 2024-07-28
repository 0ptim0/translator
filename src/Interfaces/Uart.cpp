#include "Uart.hpp"

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include "Message.hpp"

using namespace interface;

Uart::Uart() {
    strncpy(this->m_name, UART_DEFAULT_NAME, name_max_length);
    strncpy(this->m_path, UART_DEFAULT_PATH, path_max_length);
}

Uart::Uart(const char *name, const char *path) {
    if (name != nullptr) {
        strncpy(this->m_name, name, name_max_length);
    }
    if (path != nullptr) {
        strncpy(this->m_path, path, path_max_length);
    }
}

Uart::~Uart() {
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

int Uart::init() {
    this->fd = open(this->m_path, O_RDWR);
    if (this->fd < 0) {
        syslog(LOG_ERR, "Failed to open %s in %s", this->m_name, this->m_path);
        syslog(LOG_ERR, "%s", strerror(errno));
        return -1;
    }

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

    this->run();
    return 0;
}

int Uart::exec(char *cmd) {
    return 0;
}