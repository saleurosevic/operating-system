//
// Created by os on 1/25/24.
//

#include "../h/scheduler.hpp"

List<TCB> Scheduler::readyThreadQueue;

//FIFO

TCB *Scheduler::get()
{
    return readyThreadQueue.removeFirst();
}

void Scheduler::put(TCB *ccb)
{
    readyThreadQueue.addLast(ccb);
}
