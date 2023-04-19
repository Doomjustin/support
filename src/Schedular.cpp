#include "support/Schedular.h"
#include "posix/Selector.h"

#include <memory>


namespace support {

void Event::read()
{
    if (is_valid())
        read_();
}

void Event::write()
{
    if (is_valid())
        write_();
}

void Event::except()
{
    if (is_valid())
        except_();
}


void Schedular::add_event(std::shared_ptr<Event> event)
{
    events_.insert_or_assign(event->native_handler(), event);
}

void Schedular::remove_event(int event)
{
    events_.erase(event);
}

void Schedular::remove_event(std::shared_ptr<Event> event)
{
    remove_event(event->native_handler());
}

std::shared_ptr<Schedular> get_schedular()
{
    return std::make_shared<Selector>();
}

} // namespace support