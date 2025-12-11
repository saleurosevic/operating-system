//
// Created by os on 1/25/24.
//

#ifndef OS_PROJEKAT_TCB_HPP
#define OS_PROJEKAT_TCB_HPP

#include "scheduler.hpp"
#include "../lib/hw.h"
#include "syscall_c.hpp"

class TCB {
public:
    ~TCB() { thread_exit(); } //delete[] stack;

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    bool isBlocked() const { return blocked; }

    void setBlocked(bool value) { blocked = value; }

    uint64 getPid() const { return pid; }

    void setPid(uint64 pid) { TCB::pid = pid; }

    void *getArg() const { return arg; }

    void setArg(void *arg2) { arg = arg2; }

    using Body = void (*)(void *);

    static int createThread(thread_t *handle ,Body body, void* arg, uint64 *stack_space);

    static void yield();

    static TCB *running;

    static TCB *mainThread;

    static void dispatch();
private:
    //handle = new TCB(...) u thread_create iz c api
    //posto mi ovo izvrsavamo u prekidnoj rutini ne mozemo ovde pozvati mem_alloc za stek vec to moramo uraditi u syscall_c, prvo taj sistemski poziv pa onda za create_thread
    explicit TCB(Body body, void *arg, uint64 *stack_space) :
        body(body),
        arg(arg),
        pid(numOfThreads++),
        finished(false),
        blocked(false),
        stack(body != nullptr ? stack_space : nullptr),
        context({
            body != nullptr ? (uint64) &threadWrapper : 0,
            stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0
        })
    {
//        numOfThreads++;
        if (body != nullptr) { Scheduler::put(this); }
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void *arg;
    uint64 pid;
    bool finished;
    bool blocked; // za semafore sam morao da dodam
    uint64 *stack;
    Context context;
    static int numOfThreads;

    friend class Riscv; //da bi mogli da pristupamo privatnim delovima ovde

    static void threadWrapper(); //staticka metoda koja se prva izvrsava za svaku novokreiranu nit - on poziva body

    static void contextSwitch(Context *oldContext, Context *runningContext);

    // static void dispatch();

    static int exit();

    static void thread_join(thread_t handle);
};


#endif //OS_PROJEKAT_TCB_HPP
