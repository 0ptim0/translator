#include <errno.h>
#include <fcntl.h>
#include <nuttx/i2c/i2c_master.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <cstdio>
// #include "lib/libyaml/include/yaml.h"
#include "yaml.h"

int intr = 1;

void *echo_thread(void *arg) {
    int fd = open("/dev/ttyS1", O_RDWR);
    if (fd < 0) {
        printf("%s: Failed to open: %s\n", __func__, strerror(errno));
        return NULL;
    }
    struct termios termios;
    tcgetattr(fd, &termios);
    termios.c_lflag &= ~ICANON;
    termios.c_cc[VTIME] = 10;
    tcsetattr(fd, TCSANOW, &termios);
    char buf[32] = {0};
    while (intr) {
        ssize_t rb = read(fd, buf, sizeof(buf));
        if (rb < 0) {
            printf("%s: Failed to read: %s\n", __func__, strerror(errno));
            continue;
        }
        ssize_t wb = write(fd, buf, rb);
        if (wb < 0) {
            printf("%s: Failed to write: %s\n", __func__, strerror(errno));
        }
    }
    close(fd);
    return NULL;
}

void *mpu_thread(void *arg) {
    // i2c_master_s i2c;
    // i2c_config_s i2c_confg;
    // i2c_confg.address = 0x68;
    // i2c_confg.addrlen = 7;
    // i2c_confg.frequency = 100000;
    // int rv = 0;
    // uint8_t whoami = 0x75;
    // uint8_t iam = 0;
    // while (intr) {
    //     rv = i2c_writeread(&i2c, &i2c_confg, &whoami, 1, &iam, 1);
    //     if (rv) {
    //         printf("%s: Failed to write: %s\n", __func__, strerror(errno));
    //     } else {
    //         printf("I am: 0x%X\n", iam);
    //     }
    //     sleep(1);
    // }
    return NULL;
}

const unsigned char yaml_example[] = {
    "top_module:\n  type: real\n"
};

extern "C" int main(int argc, char **argv) {
  yaml_parser_t parser;
  yaml_document_t document;
  yaml_node_t *node;
  int i = 1;

  yaml_parser_initialize(&parser);
  printf("Test\n");

  yaml_parser_set_input_string(&parser, yaml_example, sizeof(yaml_example) - 1);

  if (!yaml_parser_load(&parser, &document)) {
    goto done;
  }

  while(1) {
    node = yaml_document_get_node(&document, i);
    if(!node) break;
    printf("Node [%d]: %d\n", i++, node->type);
    if(node->type == YAML_SCALAR_NODE) {
      printf("Scalar [%d]: %s\n", node->data.scalar.style, node->data.scalar.value);
    }
  }

  return 0;

done:
    // board_init();
    // signal(SIGINT, &intr_handler);
    // signal(SIGKILL, &intr_handler);
    // signal(SIGTERM, &intr_handler);
    pthread_t uart_thread = {0};
    pthread_t i2c_thread = {0};
    pthread_attr_t attr = {0};
    pthread_attr_init(&attr);
    pthread_create(&uart_thread, &attr, echo_thread, nullptr);
    pthread_create(&i2c_thread, &attr, mpu_thread, nullptr);
    pthread_join(uart_thread, NULL);
    printf("Exit\n");
    intr = 1;
    return 0;
}
