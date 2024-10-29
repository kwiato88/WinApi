//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <stdexcept>
#include <process.h> 
#include "WinApiThread.hpp"
#include "WinApiLastErrorException.hpp"

namespace WinApi
{

Thread::Thread(const std::function<void()>& p_threadFunction)
    : m_threadFunction(p_threadFunction),
      m_thread(NULL),
      m_threadId(0),
      m_running(false)
{
}

Thread::~Thread()
{
    if (m_running)
    {
        stop();
    }
}

void Thread::start()
{
    m_thread = (HANDLE)_beginthreadex(NULL, 0, threadFunction, this, 0, &m_threadId);
    if(m_thread == 0)
    {
        throw Exception("Thread creation failed");
    }
    m_running = true;
}

void Thread::join()
{
    WaitForSingleObject(m_thread, INFINITE);
    CloseHandle(m_thread);
}

void Thread::stop()
{
    TerminateThread(m_thread, 0);
    CloseHandle(m_thread);
    m_running = false;
}

unsigned __stdcall Thread::threadFunction(void* p_this)
try
{
    Thread* thread = static_cast<Thread*>(p_this);
    thread->m_threadFunction();
    return 0;
}
catch(...)
{
    return 1;
}

}
