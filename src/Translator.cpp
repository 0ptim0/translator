#include "Translator.hpp"

#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include "interfaces/Spi.hpp"
#include "interfaces/Uart.hpp"

Translator *Translator::inst = nullptr;

Translator::Translator() {}

Translator *Translator::getInstance() {
    if (inst != nullptr) {
        return inst;
    }
    return new Translator();
}

void Translator::daemon_help() {
    printf("Usage: translator [-h] [-v] [-r]\n");
    printf("Options:\n");
    printf("  -h \t Display this information\n");
    printf("  -v \t Enable verbosity\n");
    printf("  -r \t Disable verbosity\n");
}

int Translator::daemon(int argc, char **argv) {
    int opt = 0;
    while ((opt = getopt(argc, argv, "hvr")) != -1) {
        switch (opt) {
            case 'h':
                this->commander_help();
                break;
            case 'v':
                this->verbosity++;
                break;
            case 'r':
                this->verbosity = 0;
                break;
            default:
                syslog(LOG_ERR, "Unknown argument");
                this->commander_help();
                return 1;
        }
    }
    interfaces[0] = new Uart("uart1", "/dev/ttyS0", this->verbosity);
    interfaces[0]->init();
    interfaces[1] = new Spi("spi1", "/dev/spi1", this->verbosity);
    interfaces[1]->init();
    interfaces[0]->connect(interfaces[1]);
    interfaces[1]->connect(interfaces[0]);
    return 0;
}

void Translator::commander_help() {
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
                this->commander_help();
                break;
            case 'v':
                this->verbosity++;
                break;
            case 'r':
                this->verbosity = 0;
                break;
            default:
                syslog(LOG_ERR, "Unknown argument");
                this->commander_help();
                return 1;
        }
    }
    return 0;
}

int Translator::routine() {
    // syslog(LOG_INFO, "Routine");
    sleep(1);
    return 0;
}
