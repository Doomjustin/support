#ifndef CPP_SUPPORT_COLLECTIONS_HEAP_H
#define CPP_SUPPORT_COLLECTIONS_HEAP_H

#include <concepts>
#include <vector>
#include <algorithm>


namespace support {

template<typename T, typename Comparator>
class BasicHeap {
public:
    using ValueType = T;
    using SizeType = std::size_t;
    using ValueReference = ValueType&;
    using ConstValueReference = const ValueType&;

public:
    BasicHeap()
      : elements_{}
    {}

    explicit BasicHeap(SizeType count, ConstValueReference init_value = ValueType())
      : elements_(count, init_value)
    {}

    BasicHeap(std::initializer_list<T> init_list)
      : elements_{ init_list }
    {
        std::make_heap(elements_.begin(), elements_.end(), comparator_);
    }

    template<typename... Args>
        requires std::constructible_from<T, Args...>
    void emplace(Args&&... args)
    {
        elements_.emplace_back(std::forward<Args>(args)...);
        std::push_heap(elements_.begin(), elements_.end(), comparator_);
    }

    void push(ConstValueReference value)
    {
        elements_.push_back(value);
        std::push_heap(elements_.begin(), elements_.end(), comparator_);
    }

    ValueReference get() noexcept
    {
        return elements_[0];
    }

    ConstValueReference get() const noexcept
    {
        return elements_[0];
    }

    void pop()
    {
        std::pop_heap(elements_.begin(), elements_.end(), comparator_);
        elements_.pop_back();
    }

    constexpr bool empty() const noexcept { return elements_.empty(); }

private:
    Comparator comparator_;
    std::vector<T> elements_;
};

template<typename T>
using MinHeap = BasicHeap<T, std::greater<T>>;

template<typename T>
using MaxHeap = BasicHeap<T, std::less<T>>;

} // namespace support

#endif // CPP_SUPPORT_COLLECTIONS_HEAP_H