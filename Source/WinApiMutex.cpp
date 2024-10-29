//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <synchapi.h>
#include "WinApiMutex.hpp"
#include "WinApiLastErrorException.hpp"

namespace WinApi
{

Mutex::Mutex()
{
    m_mutex = CreateMutex(NULL, FALSE, NULL);
    if(m_mutex == NULL)
    {
        throw LastErrorException("Mutex creation failed");
    }
}

Mutex::~Mutex()
{
    CloseHandle(m_mutex);
}

void Mutex::lock()
{
    if(WaitForSingleObject(m_mutex, INFINITE) != WAIT_OBJECT_0)
    {
        throw Exception("Mutex lock failed");
    }
}

void Mutex::unlock()
{
    if(!ReleaseMutex(m_mutex))
    {
        throw LastErrorException("Mutex release failed");
    }
}

ScopedLock::ScopedLock(Mutex& p_mutex)
    : m_mutex(p_mutex)
{
    m_mutex.lock();
}

ScopedLock::~ScopedLock()
{
    m_mutex.unlock();
}

}
