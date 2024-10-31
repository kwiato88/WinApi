//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include <windows.h>
#include <functional>

namespace WinApi
{

class Thread
{
public:
    Thread(const std::function<void()>& p_threadFunction);
    ~Thread();

    void start();
    void join();
    void stop();
	bool isRunning() const;

private:
    static unsigned __stdcall threadFunction(void* p_this);

    std::function<void()> m_threadFunction;
    HANDLE m_thread;
    unsigned m_threadId;
    bool m_running;
};

};
