#include "Translator.hpp"

#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include "Interfaces/I2c.hpp"
#include "Interfaces/Spi.hpp"
#include "Interfaces/Uart.hpp"

Translator *Translator::inst = nullptr;

Translator::Translator() {
}

Translator *Translator::getInstance() {
    if (inst != nullptr) {
        return inst;
    }
    return new Translator();
}

int Translator::daemon(int argc, char **argv) {
    auto help = [] {
        printf("Usage: translator [-h] [-v]\n");
        printf("Options:\n");
        printf("  -h \t Display this information\n");
        printf("  -v \t Verbosity\n");
    };
    int opt = 0;
    while ((opt = getopt(argc, argv, "hv")) != -1) {
        switch (opt) {
            case 'v':
                this->dbg_messages = true;
                break;
            default:
                syslog(LOG_ERR, "Unknown argument");
            case 'h':
                help();
                return 1;
        }
    }
    syslog(LOG_DEBUG, "Running daemon...");
    interfaces[0] =
        new uart::Interface("uart2", "/dev/ttyS2", interface::Mode::READ_ONLY);
    interfaces[1] =
        new spi::Interface("spi1", "/dev/spi1", interface::Mode::WRITE_ONLY);
    interfaces[2] =
        new i2c::Interface("i2c1", "/dev/i2c1", interface::Mode::WRITE_ONLY);
    interfaces[0]->init();
    interfaces[1]->init();
    interfaces[2]->init();
    interfaces[0]->connect(interfaces[1]);
    interfaces[0]->connect(interfaces[2]);
    while (1) {
        sleep(60);
    }
    return 0;
}

void Translator::commanderHelp() {
}

int Translator::commander(int argc, char **argv) {
    auto help = [] {
        printf("Usage: translator [-h] [-i] [-c]\n");
        printf("Options:\n");
        printf("  -h \t Display this information\n");
        printf("  -i \t Interface\n");
        printf("  -c \t Command\n");
    };
    char *commander_interface = nullptr, *commander_command = nullptr;
    int opt = 0;
    while ((opt = getopt(argc, argv, "hi:c:")) != -1) {
        switch (opt) {
            case 'i':
                commander_interface = optarg;
                break;
            case 'c':
                commander_command = optarg;
                break;
            default:
                syslog(LOG_ERR, "Unknown argument");
            case 'h':
                help();
                return 1;
        }
    }
    if (commander_interface == nullptr || commander_command == nullptr) {
        syslog(LOG_ERR, "Specify both interface and command");
        help();
        return 1;
    }
    for (int i = 0; i < interface::max_interfaces; ++i) {
        if (this->interfaces[i] == nullptr) break;
        if (strncmp(interfaces[i]->name(), commander_interface,
                    interface::name_max_length) == 0) {
            interfaces[i]->configure(commander_command);
            return 0;
        }
    }
    help();
    return 1;
}
