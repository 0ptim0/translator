#include "Uart.hpp"

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include "Message.hpp"

uart::Interface::Interface(const char *name, const char *path,
                           interface::Mode mode)
    : interface::Base(name, path, mode) {
}

uart::Interface::~Interface() {
}

int uart::Interface::init() {
    int perm = this->m_mode == interface::READ_ONLY    ? O_RDONLY
               : this->m_mode == interface::WRITE_ONLY ? O_WRONLY
                                                       : O_RDWR;
    this->fd = open(this->m_path, perm);
    if (this->fd < 0) {
        syslog(LOG_ERR, "Failed to open %s in %s", this->m_name, this->m_path);
        syslog(LOG_ERR, "%s", strerror(errno));
        return -1;
    }

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 1024;
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

int uart::Interface::exec(const char *cmd) {
    return 0;
}
