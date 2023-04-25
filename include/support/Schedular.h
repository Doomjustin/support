#ifndef CPP_SUPPORT_SCHEDULAR_H
#define CPP_SUPPORT_SCHEDULAR_H

#include <functional>
#include <concepts>
#include <map>
#include <memory>


namespace support {

class Event {
public:
    using CallbackType = std::function<void()>;

public:
    ~Event() {}

    template<typename F, typename... Args>
        requires std::invocable<F, Args...>
    void on_read(F f, Args&&... args)
    {
        read_ = std::bind(std::move(f), std::forward<Args>(args)...);
    }

    template<typename F, typename... Args>
        requires std::invocable<F, Args...>
    void on_write(F f, Args&&... args)
    {
        write_ = std::bind(std::move(f), std::forward<Args>(args)...);
    }

    template<typename F, typename... Args>
        requires std::invocable<F, Args...>
    void on_except(F f, Args&&... args)
    {
        except_ = std::bind(std::move(f), std::forward<Args>(args)...);
    }

    bool readable() const noexcept { return static_cast<bool>(read_); }
    bool writable() const noexcept { return static_cast<bool>(write_); }
    bool exceptable() const noexcept { return static_cast<bool>(except_); }

    void disable_read() { read_ = nullptr; }
    void disable_write() { write_ = nullptr; }
    void disable_except() { except_ = nullptr; }

    virtual constexpr int native_handler() const noexcept = 0;

    virtual bool is_valid() const noexcept = 0;

    void read();
    void write();
    void except();

protected:
    CallbackType read_;
    CallbackType write_;
    CallbackType except_;
};


class Schedular {
public:
    ~Schedular() {}

    void add_event(std::shared_ptr<Event> event);
    void remove_event(int event);
    void remove_event(std::shared_ptr<Event> event);

    virtual void schedule() = 0;

protected:
    std::map<int, std::shared_ptr<Event>> events_;
};

std::shared_ptr<Schedular> get_schedular();

} // namespace support

#endif // CPP_SUPPORT_SCHEDULAR_H