#ifndef INTERFACE
#define INTERFACE

class Interface {
public:
    virtual int init() = 0;
    virtual int exec(char *cmd) = 0;

public:
    virtual ~Interface() = 0;
};

#endif  // INTERFACE
