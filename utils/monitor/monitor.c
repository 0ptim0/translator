#include <zephyr/debug/thread_analyzer.h>
#include <zephyr/kernel.h>
#include <zephyr/kernel/thread.h>
#include <zephyr/shell/shell.h>

#define MONITOR_STACK_SIZE 2048
#define MONITOR_BUF_SIZE   2048

static char monitor_buf[MONITOR_BUF_SIZE];
static char *monitor_buf_ptr;
static int cmd_monitor_print(const struct shell *sh, size_t argc, char **argv);
static void monitor_thread(void);

SHELL_STATIC_SUBCMD_SET_CREATE(sub_monitor,
                               SHELL_CMD(print, NULL,
                                         "Print thread information",
                                         cmd_monitor_print),
                               SHELL_SUBCMD_SET_END);
SHELL_CMD_REGISTER(monitor, &sub_monitor, "Monitor", NULL);

K_THREAD_DEFINE(monitor, MONITOR_STACK_SIZE, monitor_thread, NULL, NULL, NULL,
                K_LOWEST_APPLICATION_THREAD_PRIO, 0, 0);

void monitor_print(struct thread_analyzer_info *info) {
    int len = snprintk(monitor_buf_ptr,
                       monitor_buf + MONITOR_BUF_SIZE - monitor_buf_ptr - 1,
                       "%-16s%-11u%-11u%-5u\n", info->name, info->stack_size,
                       info->stack_used, info->utilization);
    monitor_buf_ptr += len;
}

static int cmd_monitor_print(const struct shell *sh, size_t argc, char **argv) {
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    shell_print(sh, "Thread name     Stack size Stack used Load\n%s",
                monitor_buf);
    return 0;
}

static void monitor_thread(void) {
    monitor_buf_ptr = monitor_buf;
    while (1) {
        monitor_buf_ptr = monitor_buf;
        thread_analyzer_run(monitor_print);
        k_sleep(K_SECONDS(1));
    }
}
