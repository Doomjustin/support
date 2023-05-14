#ifndef CPP_SUPPORT_COLLECTION_LIST_H
#define CPP_SUPPORT_COLLECTION_LIST_H

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>


namespace support {

// 自动扩容数组
template<typename T, typename AllocatorType = std::allocator<T>>
class List {
public:
    using Value = T;
    using Allocator = AllocatorType;
    using Difference = std::ptrdiff_t;
    using Size = std::size_t;
    using Reference = Value&;
    using ConstReference = const Value&;
    using Pointer = Value*;
    using ConstPointer = const Value*;
    using Iterator = Value*;
    using ConstIterator = const Value*;

public:
    List()
      : allocator_{},
        start_of_storage_{ std::allocator_traits<Allocator>::allocate(allocator_, default_capacity) },
        element_{ start_of_storage_ },
        end_of_storage_{ start_of_storage_ + default_capacity }
    {}

    List(Size init_capacity, ConstReference init_value = Value{})
      : allocator_{},
        start_of_storage_{ std::allocator_traits<Allocator>::allocate(allocator_, init_capacity) },
        element_{ start_of_storage_ + init_capacity },
        end_of_storage_{ start_of_storage_ + init_capacity }
    {
        std::uninitialized_fill(start_of_storage_, element_, init_value);
    }

    List(std::initializer_list<Value> init_list)
      : allocator_{},
        start_of_storage_{ std::allocator_traits<Allocator>::allocate(allocator_, init_list.size()) },
        element_{ start_of_storage_ + init_list.size() },
        end_of_storage_{ start_of_storage_ + init_list.size() }
    {
        // WARNING: move会造成源对象失效
        std::move(init_list.begin(), init_list.end(), start_of_storage_);
    }

    List(const List& other)
      : allocator_{ other.allocator_ },
        start_of_storage_{ std::allocator_traits<Allocator>::allocate(allocator_, other.capacity()) },
        element_{ start_of_storage_ + other.size() },
        end_of_storage_{ start_of_storage_ + other.capacity() }
    {
        std::uninitialized_copy(other.begin(), other.end(), start_of_storage_);
    }

    List& operator=(const List& other)
    {
        if (this == &other)
            return *this;

        auto tmp{ other };
        std::swap(tmp, *this);

        return *this;
    }

    List(List&& other) noexcept
      : allocator_{ std::move(other.allocator_) },
        start_of_storage_{ std::move(other.start_of_storage_) },
        element_{ std::move(other.element_) },
        end_of_storage_{ std::move(other.end_of_storage_) }
    {
        // 确保源对象处于无效状态
        other.start_of_storage_ = other.element_ = other.end_of_storage_ = nullptr;
    }

    List& operator=(List&& other) noexcept
    {
        if (this == &other)
            return *this;

        auto tmp = std::move(other);
        std::swap(*this, tmp);

        return *this;
    }

    ~List()
    {
        clear();
        destroy();
    }

    void resize(Size new_size)
    {
        if (new_size == size())
            return;
        else if (new_size > size())
            expand(new_size);
        else
            shrink(new_size);
    }

    void push_back(ConstReference value)
    {
        auto_expand_if_overflow();

        *element_++ = value;
    }

    void insert(Size pos, ConstReference value)
    {
        auto_expand_if_overflow();

        std::move_backward(begin() + pos, end(), end() + 1);
        *(start_of_storage_ + pos) = value;
        ++element_;
    }

    // 仅删除对象，不删除memory
    void clear()
    {
        std::destroy(begin(), end());
    }

    constexpr Size size() const noexcept { return element_ - start_of_storage_; }
    constexpr Size capacity() const noexcept { return end_of_storage_ - start_of_storage_; }
    constexpr bool empty() const noexcept { return element_ == start_of_storage_; }

    // 执行越界检查
    Reference at(Size pos)
    {
        is_overflow(pos);
        return *(start_of_storage_ + pos);
    }

    ConstReference at(Size pos) const
    {
        is_overflow(pos);
        return *(start_of_storage_ + pos);
    }

    Reference operator[](Size pos) noexcept
    {
        return *(start_of_storage_ + pos);
    }

    ConstReference operator[](Size pos) const noexcept
    {
        return *(start_of_storage_ + pos);
    }

    Reference front() noexcept { return *start_of_storage_; }
    ConstReference front() const noexcept { return *start_of_storage_; }

    Reference back() noexcept { return *(element_ - 1); }
    ConstReference back() const noexcept { return *(element_ - 1); }

    Iterator begin() noexcept { return start_of_storage_; }
    ConstIterator begin() const noexcept { return start_of_storage_; }
    Iterator end() noexcept { return end_of_storage_; }
    ConstIterator end() const noexcept { return end_of_storage_; }

    ConstIterator cbegin() noexcept { return start_of_storage_; }
    ConstIterator cbegin() const noexcept { return start_of_storage_; }
    ConstIterator cend() noexcept { return end_of_storage_; }
    ConstIterator cend() const noexcept { return end_of_storage_; }

private:
    static std::uint8_t default_capacity;             // 默认初始化时的容量大小
    static unsigned int default_expand_coefficient;   // 默认扩容系数

    Allocator allocator_;
    //    |------|__________|
    //    | 对象 | 未构建对象|
    // start   element     end
    Pointer start_of_storage_;
    Pointer element_;
    Pointer end_of_storage_;

    // 检查pos是否越界
    void is_overflow(Size pos) const
    {
        if (pos >= size())
            throw std::out_of_range{ "List overflow" };
    }

    // 销毁已分配对象
    void destroy()
    {
        std::allocator_traits<Allocator>::destroy(allocator_, start_of_storage_);
        end_of_storage_ = element_ = start_of_storage_ = nullptr;
    }

    void auto_expand_if_overflow()
    {
        if (size() < capacity())
            return;

        auto tmp_size = size();
        auto new_memory = std::allocator_traits<Allocator>::allocate(allocator_,
                                                                     default_expand_coefficient * capacity());

        std::uninitialized_move(begin(), end(), new_memory);
        destroy();

        start_of_storage_ = new_memory;
        element_ = start_of_storage_ + tmp_size;
        end_of_storage_ = start_of_storage_ + tmp_size * default_expand_coefficient;
    }

    // 收缩容积
    // 前提: size() > new_size
    void shrink(Size new_size)
    {
        auto new_memory = std::allocator_traits<Allocator>::allocate(allocator_, new_size);
        std::move(start_of_storage_, start_of_storage_ + size(), new_memory);
        destroy();

        start_of_storage_ = new_memory;
        element_ = end_of_storage_ = start_of_storage_ + new_size;
    }

    // 扩大容积
    // 前提: size() < new_size
    void expand(Size new_size)
    {
        auto new_storage = std::allocator_traits<Allocator>::allocate(allocator_, new_size);
        std::move(begin(), end(), new_storage);
        std::uninitialized_fill(new_storage + size(), new_storage + new_size, Value{});
        destroy();

        start_of_storage_ = new_storage;
        element_ = end_of_storage_ = start_of_storage_ + new_size;
    }
};

template<typename T, typename AllocatorType>
std::uint8_t List<T, AllocatorType>::default_capacity = 4;

template<typename T, typename AllocatorType>
unsigned int List<T, AllocatorType>::default_expand_coefficient = 2;

} // namespace support

#endif // CPP_SUPPORT_COLLECTION_LIST_H