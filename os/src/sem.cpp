//
// Created by os on 1/28/24.
//

#include "../h/sem.hpp"

#include "../h/MemoryAllocator.hpp"

// List<TCB> Sem::blockedThreadsQueue;

int Sem::sem_open(Sem** handle, unsigned init)
{
    *handle = new Sem((int)init); //ovo ne znam dal sme u principu u svim ovim koje se izvrsavaju
    // Sem* sem_tmp = (Sem*)MemoryAllocator::mem_alloc(sizeof(Sem));
    // sem_tmp->val = init;
    // *handle = sem_tmp;
    if(handle == nullptr)
        return 0;
    else
        return 1;
}

int Sem::sem_close(Sem* handle) //dodaj ono za gresku, ideja mi je da ovde menjam jednu promenljivu error i na kraju wait proveravam njenu vrednost i dal vraca 0 ili -1
{
    if(handle == nullptr) return -1;
    int numOfBlockedThreads = handle->blockedThreadsQueue.getLength();
    if(numOfBlockedThreads > 0){
        for (int i = 0; i < numOfBlockedThreads; ++i) {
            TCB* myThread = handle->blockedThreadsQueue.removeFirst();
            Scheduler::put(myThread);
        }
    }
    return 0;
}

int Sem::sem_wait(Sem* id) //uspeh 0, neuspeh -1, probaj while umesto if provere za blocked i val i pre while postavi blocked na false;
{
    if(id == nullptr) return -1;
    id->val--;
    if(id->val < 0){ //blokira se i samo predaje dalje procesor ali ne moze da ide u red spremnih dok se ne odblokira
        if(!id->blockedThreadsQueue.contain(TCB::running)) {
            id->blockedThreadsQueue.addLast(TCB::running);
        }
        TCB::running->setBlocked(true);
        // thread_dispatch(); OVO JE BILA GRESKA -> NE MOZE IZ SISTESMKOG REZIMA DA PRISTUPA OVDE
        TCB::dispatch();
    }
    return 0;
}

int Sem::sem_signal(Sem* id)
{
    if(id == nullptr) return -1;
    id->val++;
    TCB* myThread = id->blockedThreadsQueue.removeFirst();
    if(myThread != nullptr){ //imalo je blokiranih i ovako proveravam da li je val > 0 ili nije, ako ima blokiranih => nije vece
        myThread->setBlocked(false);
        Scheduler::put(myThread);
    }
    return 0;
}

int Sem::sem_trywait(Sem *id) {
    if(id == nullptr) return -1;
    id->val--;
    if(id->val < 0) return 0;
    return 1;
}


