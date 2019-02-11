#include "cr-sync-semaphore.hpp"
#include <thread>
gearoenix::core::sync::Semaphore::Semaphore(int count)
    : count(count)
{
}

gearoenix::core::sync::Semaphore::~Semaphore() {}

void gearoenix::core::sync::Semaphore::lock()
{
    m_count.lock();
    if (count > 0) {
        --count;
        m_count.unlock();
        return;
    }
    Lock l(new LockData);
    q.push(l);
    std::unique_lock<std::mutex> lock(l->m);
    m_count.unlock();
    l->c.wait(lock);
    l->locked = false;
}

void gearoenix::core::sync::Semaphore::release()
{
    std::lock_guard<std::mutex> lock(m_count);
    if (q.size() < 1) {
        ++count;
        return;
    }
    Lock& l = q.front();
    do {
        l->c.notify_all();
        std::this_thread::yield();
    } while (l->locked);
    q.pop();
}