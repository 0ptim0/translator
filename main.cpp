#include <nuttx/config.h>
#include <nuttx/init.h>
#include <stdio.h>
#include <syslog.h>

#include "Translator.hpp"

Translator t;
bool daemon_flag = false;

static int daemon_task(int argc, char *argv[]) {
    t.init();
    while (1) {
        sleep(1);
        syslog(LOG_INFO, "Daemon strob");
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
        syslog(LOG_INFO, "Here should be the translator commander %d", daemon_flag);
    }
    return 0;
}
}
