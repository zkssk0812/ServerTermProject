#pragma once
#ifndef _ThreadSafeQueue

#include <windows.h>
#include <queue>

template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() {
        InitializeCriticalSection(&m_cs);
        InitializeConditionVariable(&m_cv);
    }
    ~ThreadSafeQueue() {
        DeleteCriticalSection(&m_cs);
    }
    void Push(T value);
    T Pop();

private:
    std::queue<T> m_queue;
    CRITICAL_SECTION m_cs;
    CONDITION_VARIABLE m_cv;
};

#endif 
