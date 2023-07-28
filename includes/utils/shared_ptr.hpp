#pragma once

#include <ostream>

namespace ft
{
template <typename T>
class shared_ptr
{
  private:
    T *ptr;
    int *count;

  public:
    explicit shared_ptr(T *p = nullptr) : ptr(p), count(new int(1)) {}

    ~shared_ptr()
    {
        --(*count);
        if (*count == 0)
        {
            delete ptr;
            delete count;
        }
    }

    shared_ptr(const shared_ptr &other) : ptr(other.ptr), count(other.count)
    {
        ++(*count);
    }

    shared_ptr &operator=(const shared_ptr &other)
    {
        if (this != &other)
        {
            --(*count);
            if (*count == 0)
            {
                delete ptr;
                delete count;
            }
            ptr = other.ptr;
            count = other.count;
            ++(*count);
        }
        return *this;
    }

    T &operator*() { return *ptr; }
    T *operator->() { return ptr; }
    T *getPtr() { return ptr; }
    int getCount() { return *count; }

    friend std::ostream &operator<<(std::ostream &os, ft::shared_ptr<T> &sp)
    {
        os << "Address pointed : " << sp.getPtr() << std::endl;
        os << *(sp.count) << std::endl;
        return os;
    }
};

} // namespace ft
