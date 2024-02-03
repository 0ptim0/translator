#ifndef UART
#define UART

#include "Interface.hpp"

constexpr const char UART_DEFAULT_PATH[] = "/dev/ttyS1";
constexpr const char UART_DEFAULT_NAME[] = "uart0";

class Uart : public Interface {
private:
    unsigned baudrate = 115200;

public:
    Uart();
    Uart(const char *name, const char *path, int verbosity);
    ~Uart();

public:
    virtual int init() override;
    virtual int exec(char *cmd) override;
};

#endif  // UART
