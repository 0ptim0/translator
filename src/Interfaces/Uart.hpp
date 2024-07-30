#ifndef UART
#define UART

#include "Base.hpp"

namespace uart {

class Interface : public interface::Base {
private:
    unsigned baudrate = 115200;

public:
    Interface(const char *name, const char *path, interface::Mode mode);
    ~Interface();

public:
    virtual int init() override;
    virtual int exec(const char *cmd) override;
};
}  // namespace uart

#endif  // UART
