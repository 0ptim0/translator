#ifndef UART
#define UART

#include "Interface.hpp"

constexpr int UART_MAX_PATH_LENGTH = 32;
constexpr const char UART_DEFAULT_PATH[] = "/dev/ttyS1";

class Uart : public Interface {
private:
    unsigned baudrate = 115200;
    char path[UART_MAX_PATH_LENGTH] = {0};
    int fd = -1;

public:
    Uart();
    ~Uart();

public:
    virtual int init() override;
    virtual int exec(char *cmd) override;

public:
    static void *handler(void *handler);
};

#endif // UART
