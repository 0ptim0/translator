#ifndef TRANSLATOR
#define TRANSLATOR

#include "InterfaceBase.hpp"

class Translator {
private:
    static Translator *inst;

private:
    interface::InterfaceBase *interfaces[interface::max_interfaces] = {0};

private:
    Translator();

public:
    Translator(const Translator &) = delete;
    void operator=(const Translator &) = delete;

public:
    static Translator *getInstance();

public:
    int daemon(int argc, char **argv);
    void daemonHelp();

public:
    int commander(int argc, char **argv);
    void commanderHelp();

public:
    int routine();

private:
    int verbosity = 0;
};

#endif  // TRANSLATOR
