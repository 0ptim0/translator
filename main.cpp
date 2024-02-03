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
    while (1) {
        translator->routine();
    }
    return 0;
}

extern "C" {
int main(int argc, char **argv) {
    if (translator == nullptr) {
        int pid = task_create("tl_daemon", SCHED_PRIORITY_DEFAULT, 2048,
                              daemon_task, argv);
        (void)pid;
    } else {
        translator->commander(argc, argv);
    }
    return 0;
}
}
