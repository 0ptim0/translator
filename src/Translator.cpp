#include "Translator.hpp"

#include <string.h>
#include <syslog.h>

#include "Uart.hpp"

Translator *Translator::inst = nullptr;

Translator::Translator() {
    syslog(LOG_INFO, "Translator ctr");
    strncpy(this->path, TRANSLATOR_CFG_DEFAULT_PATH,
            TRANSLATOR_CFG_MAX_PATH_LENGTH - 1);
    // this->fd = open(this->path, O_RDWR);
    // if (this->fd < 0) {
    //     return -1;
    // }
    interfaces[0] = new Uart();
    interfaces[0]->init();
}

Translator *Translator::getInstance() {
    if (inst != nullptr) {
        return inst;
    }
    return new Translator();
}

int Translator::commander(int argc, char **argv) {
    return 0;
}
