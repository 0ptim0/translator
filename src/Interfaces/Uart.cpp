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

    this->run();
    return 0;
}

int uart::Interface::exec(const char *command) {
    syslog(LOG_DEBUG, "%s: command: %s", this->name(), command);
    return 0;
}
