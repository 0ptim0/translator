#ifndef UART
#define UART

#include "InterfaceBase.hpp"

constexpr const char UART_DEFAULT_PATH[] = "/dev/ttyS1";
constexpr const char UART_DEFAULT_NAME[] = "uart0";

class Uart : public interface::InterfaceBase {
private:
    unsigned baudrate = 115200;

public:
    Uart(const char *name, const char *path, interface::Mode mode);
    ~Uart();

public:
    virtual int init() override;
    virtual int exec(const char *cmd) override;
};

#endif  // UART
