#include "Translator.hpp"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <getopt.h>

#include "interfaces/Uart.hpp"
#include "interfaces/Spi.hpp"

Translator *Translator::inst = nullptr;

Translator::Translator() {
    interfaces[0] = new Uart();
    interfaces[0]->init();
    interfaces[1] = new Spi();
    interfaces[1]->init();
}

Translator *Translator::getInstance() {
    if (inst != nullptr) {
        return inst;
    }
    return new Translator();
}

void Translator::help() {
    printf("Usage: translator [-h] [-v] [-r]\n");
    printf("Options:\n");
    printf("  -h \t Display this information\n");
    printf("  -v \t Enable verbosity\n");
    printf("  -r \t Disable verbosity\n");
}

int Translator::commander(int argc, char **argv) {
    int opt = 0;
    while ((opt = getopt(argc, argv, "hvr")) != -1) {
        switch (opt) {
            case 'h':
                this->help();
                break;
            case 'v':
                this->verbosity++;
                break;
            case 'r':
                this->verbosity = 0;
                break;
            default:
                syslog(LOG_ERR, "Unknown argument");
                this->help();
                return 1;
        }
    }
    return 0;
}

int Translator::routine() {
    if (this->verbosity) {
        syslog(LOG_INFO, "Routine");
    }
    sleep(1);
    return 0;
}
