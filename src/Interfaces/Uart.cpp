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
    auto help = [] {
        printf("Available options:\n");
        for (size_t i = 0; i < sizeof(commands) / sizeof(command[0]); ++i) {
            printf("\t%s\t - %s\n", commands[i].opt, commands[i].desc);
        }
    };

    syslog(LOG_DEBUG, "%s: command: %s", this->name(), command);
    if (strstr(command, "help")) {
        help();
        return 0;
    }

    char *r = NULL;
    char *token = strtok_r(const_cast<char *>(command), ",", &r);

    if (token == NULL) {
        if (strlen(command)) {
            token = const_cast<char *>(command);
        } else {
            help();
            return -1;
        }
    }

    printf("%lu, %lu\n", sizeof(commands), sizeof(commands[0]));

    while (token != NULL) {
        char *option = NULL;
        char *value = NULL;
        char *ropt = NULL;
        option = strtok_r(token, "=", &ropt);
        value = strtok_r(NULL, "=", &ropt);
        for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); ++i) {
            if (strncmp(option, commands[i].opt, MAX_INPUT) == 0) {
                syslog(LOG_DEBUG, "%s: executing command: %s", this->name(),
                       command);
                commands[i].handler(this, value);
            }
        }
        token = strtok_r(NULL, ",", &r);
    }
    return 0;
}

int uart::Interface::setBaud(void *inst, void *arg) {
    auto t = reinterpret_cast<uart::Interface *>(inst);
    syslog(LOG_DEBUG, "%s: executing: %s", t->name(), __func__);
    t->baudrate = atoi(reinterpret_cast<const char *>(arg));
    int rv = tcgetattr(t->fd, &t->termios);
    rv |= cfsetspeed(&t->termios, t->baudrate);
    if (rv < 0) {
        syslog(LOG_ERR, "%s: failed: %s", t->name(), __func__);
        return -1;
    }
    return 0;
}

int uart::Interface::setStopBits(void *inst, void *arg) {
    auto t = reinterpret_cast<uart::Interface *>(inst);
    syslog(LOG_DEBUG, "%s: executing: %s", t->name(), __func__);
    t->stop_bits = atoi(reinterpret_cast<const char *>(arg));
    return 0;
}
