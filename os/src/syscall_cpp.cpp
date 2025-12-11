//
// Created by os on 1/23/24.
//

#include "../h/syscall_cpp.hpp"
//#include "../lib/hw.h"

void *operator new(size_t n)
{
    return mem_alloc(n);
}

void *operator new[](size_t n)
{
    return mem_alloc(n);
}

void operator delete(void *p)
{
    mem_free(p);
}

void operator delete[](void *p)
{
    mem_free(p);
}

//------------------------------------------------------------

Thread::Thread(void (*body)(void *), void *arg)
{
    this->body = body;
    this->arg = arg;
}

Thread::~Thread()
{
    thread_exit();
}

int Thread::start() //pokrece metodu run
{
    if(myHandle)
        return myHandle->getPid();
    else
        return thread_create(&myHandle, &wrapper, this);
}

void Thread::join()
{
    thread_join(myHandle);
}

void Thread::dispatch()
{
    thread_dispatch();
}

int Thread::sleep(time_t t)
{
    return 0;
}

//------------------------------------------------------------

Semaphore::Semaphore (unsigned init) //kreira semafor pozivam sem_open
{
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore ()
{
    sem_close(myHandle);
}

int Semaphore::wait()
{
    return sem_wait(myHandle);
}

int Semaphore::signal()
{
    return sem_signal(myHandle);
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}


//------------------------------------------------------------

char Console::getc() {return OS_PROJEKAT_SYSCALL_C_HPP::getc(); }

void Console::putc(char c) { OS_PROJEKAT_SYSCALL_C_HPP::putc(c); }
