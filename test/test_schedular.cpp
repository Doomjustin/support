#include "support/Schedular.h"

#include <cstdlib>


int main()
{
    auto schedular = support::get_schedular();

    schedular->schedule();

    return EXIT_SUCCESS;
}