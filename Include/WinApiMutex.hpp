//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include <Windows.h>

namespace WinApi
{

class Mutex
{
public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();
private:
    HANDLE m_mutex;
};

class ScopedLock
{
public:
    ScopedLock(Mutex& p_mutex);
    ~ScopedLock();
private:
    Mutex& m_mutex;
};

}
