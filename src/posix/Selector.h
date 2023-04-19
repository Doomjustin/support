#ifndef CPP_SUPPORT_POSIX_SELECTOR_H
#define CPP_SUPPORT_POSIX_SELECTOR_H

#include "support/Schedular.h"

#include <sys/select.h>


namespace support {

class Selector: public Schedular {
public:
    ~Selector() {}

    void schedule() override;

private:
    int max_fd_;
    fd_set read_fds_;
    fd_set write_fds_;
    fd_set except_fds_;

private:
    void update();
    void dispatch();
};

} // namespace support

#endif // CPP_SUPPORT_POSIX_SELECTOR_H