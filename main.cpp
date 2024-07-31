#include <nuttx/config.h>
#include <nuttx/init.h>
#include <stdio.h>
#include <syslog.h>

#include "Translator.hpp"

Translator *translator = nullptr;

static int daemon_task(int argc, char *argv[]) {
    translator = Translator::getInstance();
    if (translator == nullptr) {
        syslog(LOG_ERR, "Failed to allocate main obj");
        return -1;
    }
    translator->daemon(argc - 1, argv + 1);
    return 0;
}

extern "C" {
int main(int argc, char **argv) {
    if (translator == nullptr) {
        int pid = task_create("translator_daemon", SCHED_PRIORITY_DEFAULT, 4096,
                              daemon_task, argv);
        (void)pid;
    } else {
        translator->commander(argc, argv);
    }
    return 0;
}
}
