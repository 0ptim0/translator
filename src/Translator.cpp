#include "Translator.hpp"
#include "Uart.hpp"

#include <syslog.h>
#include <string.h>

Translator::Translator() {
    syslog(LOG_INFO, "Translator ctr");
    interfaces[0] = new Uart();
}

Translator::~Translator() {
    delete interfaces[0];
    syslog(LOG_INFO, "Translator dtr");
}

int Translator::init() {
    syslog(LOG_INFO, "Translator init");
    strncpy(this->path, TRANSLATOR_CFG_DEFAULT_PATH,
            TRANSLATOR_CFG_MAX_PATH_LENGTH - 1);
    // this->fd = open(this->path, O_RDWR);
    // if (this->fd < 0) {
    //     return -1;
    // }
    interfaces[0]->init();
    return 0;
}
