#pragma once

#include <sstream>

// template 함수는 헤더에 구현을 해두는게 일반적
namespace ft
{
template < typename T >
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

    shared_ptr(const shared_ptr &other) : ptr(other.ptr), count(other.count) { ++(*count); }

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

    friend std::stringstream &operator<<(std::stringstream &iss, ft::shared_ptr< T > &sp)
    {
        iss << "Address pointed : " << sp.getPtr() << "\n"
            << "count: " << *(sp.count) << "\n";
        return iss;
    }
};

} // namespace ft
