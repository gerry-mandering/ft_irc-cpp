#ifndef SINGLETON_TEMPLATE_H
#define SINGLETON_TEMPLATE_H

#include <cstddef>
#include <cstdlib>

template<class T>
class TSingleton {

public:
    static T *GetInstance() {
        if (mInstance == NULL) {
            mInstance = new T;
            std::atexit(DeleteInstance);
        }
        return mInstance;
    }
    static void DeleteInstance() {
        if (mInstance != NULL)
            delete mInstance;
        mInstance = NULL;
    }

protected:
    TSingleton() {}
    ~TSingleton() {}

private:
    static T *mInstance;
};

template<class T> T *TSingleton<T>::mInstance = NULL;

#endif