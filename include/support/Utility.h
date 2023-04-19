#ifndef CPP_SUPPORT_UTILITY_H
#define CPP_SUPPORT_UTILITY_H

#include <concepts>
#include <utility>


namespace support {

// 只单纯检测能否copy
template<class T>
concept copyable = std::copy_constructible<T> &&
                   std::assignable_from<T&, T&> &&
                   std::assignable_from<T&, const T&> &&
                   std::assignable_from<T&, const T>;

// 可单例化的类型T，需要满足不可copy但是可移动
template<typename T, typename... Args>
concept singletonable = std::constructible_from<T, Args...> &&
                        std::movable<T> &&
                        !copyable<T>;

// 多数时候，单例模式没有什么用，如果你用了，大概率是设计的不好
template<typename T, typename... Args>
    requires singletonable<T, Args...>
T& make_singleton(Args&&... args)
{
    static T singleton { std::forward<Args>(args)... };

    return singleton;
}

// 继承此基类以删除默认copy语义，得益于空基类优化，此继承不会有额外开销
struct Noncopyable {
public:
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;

    Noncopyable(Noncopyable&&) noexcept = default;
    Noncopyable& operator=(Noncopyable&&) noexcept = default;

protected:
    Noncopyable() = default;
    ~Noncopyable() {}
};

} // namespace support

#endif // CPP_SUPPORT_UTILITY_H