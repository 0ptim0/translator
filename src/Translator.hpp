#ifndef TRANSLATOR
#define TRANSLATOR

#include "Base.hpp"

class Translator {
private:
    static Translator *inst;
    interface::Base *interfaces[interface::max_interfaces] = {0};

private:
    Translator();

public:
    Translator(const Translator &) = delete;
    void operator=(const Translator &) = delete;

public:
    static Translator *getInstance();

public:
    int daemon(int argc, char **argv);
    int commander(int argc, char **argv);

private:
    bool dbg_messages = false;
};

#endif  // TRANSLATOR
