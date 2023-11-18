#include <nuttx/config.h>
#include <nuttx/init.h>
#include <stdio.h>
#include <syslog.h>

#include "Translator.hpp"

Translator *translator;
bool daemon_flag = false;

static int daemon_task(int argc, char *argv[]) {
    translator = Translator::getInstance();
    if (translator == nullptr) {
        return -1;
    }
    while (1) {
        sleep(1);
    }
    return 0;
}

extern "C" {
int main(int argc, char **argv) {
    if (!daemon_flag) {
        daemon_flag = true;
        int pid = task_create("daemon", SCHED_PRIORITY_DEFAULT, 2048,
                              daemon_task, NULL);
        (void)pid;
    } else {
        translator->commander(argc, argv);
    }
    return 0;
}
}
