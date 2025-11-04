#include "ThreadSafeQueue.h"


template <typename T>
void ThreadSafeQueue<T>::Push(T value) {
    EnterCriticalSection(&m_cs);

    m_queue.push(std::move(value));

    LeaveCriticalSection(&m_cs);


    WakeConditionVariable(&m_cv);
}

template <typename T>
T ThreadSafeQueue<T>::Pop() {

    EnterCriticalSection(&m_cs);


    while (m_queue.empty()) {

        SleepConditionVariableCS(&m_cv, &m_cs, INFINITE);
    }


    T value = std::move(m_queue.front());
    m_queue.pop();


    LeaveCriticalSection(&m_cs);

    return value;
}