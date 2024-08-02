#ifndef UART
#define UART

#include "Base.hpp"
#include "Cli.hpp"

namespace uart {

class Interface : public interface::Base {
private:
    unsigned baudrate = 115200;
    uint8_t stop_bits = 1;

public:
    Interface(const char *name, const char *path, interface::Mode mode);
    ~Interface();

public:
    virtual int init() override;
    virtual int exec(const char *command) override;

private:
    static int setBaud(void *inst, void *arg);
    static int setStopBits(void *inst, void *arg);

    static constexpr const cli::Command commands[] = {
        {"baud", setBaud, "Set baudrate"},
        {"sbits", setStopBits, "Set stopbits"}};
};
}  // namespace uart

#endif  // UART
