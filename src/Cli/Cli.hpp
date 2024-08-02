#ifndef CLI_H
#define CLI_H

namespace cli {
using Handler = int (*)(void *inst, void *arg);

struct Command {
    const char *opt;
    Handler handler;
    const char *desc;
};
}  // namespace cli

#endif  // CLI_H
