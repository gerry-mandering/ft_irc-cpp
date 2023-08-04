#pragma once

#include <iostream>

template < typename T >
class SharedPtr
{
  public:
    SharedPtr() : mPtr(NULL), mReferenceCount(NULL) {}

    ~SharedPtr() { release(); }

    explicit SharedPtr(T *ptr) : mPtr(ptr), mReferenceCount(new int(1)) {}

    SharedPtr(const SharedPtr< T > &other) : mPtr(other.mPtr), mReferenceCount(other.mReferenceCount)
    {
        if (mReferenceCount)
            ++(*mReferenceCount);
    }

    SharedPtr< T > &operator=(const SharedPtr< T > &other)
    {
        if (this != &other)
        {
            release();
            mPtr = other.mPtr;
            mReferenceCount = other.mReferenceCount;
            if (mReferenceCount)
                (*mReferenceCount)++;
        }
        return *this;
    }

    T &operator*() const { return *mPtr; }

    T *operator->() const { return mPtr; }

    T *GetPtr() const { return mPtr; }

    friend std::ostream &operator<<(std::ostream &os, SharedPtr< T > &sharedPtr)
    {
        os << "Address pointed : " << sharedPtr.getPtr() << "\n"
           << "count: " << *(sharedPtr.count) << "\n";

        return os;
    }

  private:
    void release()
    {
        if (!mReferenceCount)
            return;

        --(*mReferenceCount);
        if (*mReferenceCount == 0)
        {
            delete mPtr;
            delete mReferenceCount;
            mPtr = NULL;
            mReferenceCount = NULL;
        }
    }

    T *mPtr;
    int *mReferenceCount;
};