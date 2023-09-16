// #include <zephyr/drivers/gpio.h>
// #include <zephyr/kernel.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

int main() {
    // gpio_is_ready_dt(&led);
    // gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    // while (1) {
    //     gpio_pin_toggle_dt(&led);
    //     sleep(1);
    // }
    int fd = open("/dev/usart2", O_RDWR);
    while (1) {
        char check[] = "Hello\n";
        write(fd, check, sizeof(check));
        sleep(1);
    }
    return 0;
}
