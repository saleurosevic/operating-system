//
// Created by os on 1/25/24.
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

int TCB::numOfThreads = 1;
TCB *TCB::running = nullptr;
TCB *TCB::mainThread = nullptr;

int TCB::createThread(thread_t *handle, TCB::Body body, void *arg, uint64 *stack_space)
{
    *handle = new TCB(body, arg, stack_space);
    if(*handle == nullptr)
        return -1;
    else
        return 0;
}

void TCB::yield()
{
    __asm__ volatile ("ecall");
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie(); //pop stvari iz sstatusa SPP - stare privilegije SPIE - prekidno dal su omoguceni ili ne, jer ova nova nit nece nastaviti dalje sa izvrsavanjem supervisorTrap pa nece imati i ono sstatus read
    running->body(running->getArg()); //kada dodje ova nasa na red za izvrsavanje u dispatch() ce biti podesena da je ona running
//    thread_exit();
    running->setFinished(true);
    thread_dispatch();
}

int TCB::exit()
{
    if(running == mainThread) return -1;
    running->setFinished(true);
    mem_free(running->stack);
    thread_dispatch();
    return 0;
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && old != nullptr && !old->blocked) { Scheduler::put(old); }
    running = Scheduler::get();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::thread_join(thread_t handle)
{
    while (!handle->finished) { //blokira se ovde, ako ponovo dodje na red odblokirace se proverice ovaj uslov i predati procesor nekom drugom
        thread_dispatch();
//        if(handle == (thread_t )0x1) continue;
    }
}




