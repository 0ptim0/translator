#include "Translator.hpp"

#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include "Interfaces/Spi.hpp"
#include "Interfaces/Uart.hpp"
#include "Interfaces/I2c.hpp"

Translator *Translator::inst = nullptr;

Translator::Translator() {}

Translator *Translator::getInstance() {
    if (inst != nullptr) {
        return inst;
    }
    return new Translator();
}

void Translator::daemonHelp() {
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
                this->daemonHelp();
                break;
            case 'v':
                this->verbosity++;
                break;
            case 'r':
                this->verbosity = 0;
                break;
            default:
                syslog(LOG_ERR, "Unknown argument");
                this->daemonHelp();
                return 1;
        }
    }
    interfaces[0] = new uart::Interface("uart2", "/dev/ttyS2", interface::Mode::READ_ONLY);
    interfaces[1] = new spi::Interface("spi1", "/dev/spi1", interface::Mode::WRITE_ONLY);
    interfaces[2] = new i2c::Interface("i2c1", "/dev/i2c1", interface::Mode::WRITE_ONLY);
    interfaces[0]->init();
    interfaces[1]->init();
    interfaces[2]->init();
    interfaces[0]->connect(interfaces[1]);
    interfaces[0]->connect(interfaces[2]);
    return 0;
}

void Translator::commanderHelp() {
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
                this->commanderHelp();
                break;
            case 'v':
                this->verbosity++;
                break;
            case 'r':
                this->verbosity = 0;
                break;
            default:
                syslog(LOG_ERR, "Unknown argument");
                this->commanderHelp();
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
