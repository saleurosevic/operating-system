//
// Created by os on 1/23/24.
//

#ifndef OS_PROJEKAT_SYSCALL_CPP_HPP
#define OS_PROJEKAT_SYSCALL_CPP_HPP

#include "../lib/hw.h"
#include "syscall_c.hpp"
#include "tcb.hpp"

void* operator new (size_t);
void* operator new[] (size_t);
void operator delete (void*);
void operator delete[](void *tmp);


class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread();

    int start();

    void join();

    static void dispatch();
    static int sleep(time_t);

protected:
    Thread() { myHandle = nullptr;}
    virtual void run() {}

private:
    thread_t myHandle;
    void (*body)(void*); void* arg;

    static void wrapper(void* param) { //Thread kreiramo preko (body, arg), param mi predstavlja nit za koju kreiramo
        if(param)
            ((Thread*)param)->run();
    }
};


class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
    int tryWait();
private:
    sem_t myHandle; //oznaka semafora sem_t = Sem*
};


class PeriodicThread : public Thread {
public:
    void terminate ();
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
private:
    time_t period;
};


class Console {
public:
    static char getc ();
    static void putc (char);
};




#endif //OS_PROJEKAT_SYSCALL_CPP_HPP
