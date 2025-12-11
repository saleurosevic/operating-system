//
// Created by os on 1/28/24.
//

#ifndef OS_PROJEKAT_SEM_HPP
#define OS_PROJEKAT_SEM_HPP

#include "tcb.hpp"
#include "list.hpp"

//septembar 2015 - 145 strana / 148 / 150
//moram imati listu blokiranih na nekom odredjenom semaforu koji se blokiraju kod operacije wait
//signal ide u tu listu blokiranih i dohvata proces po proces budi ih i oni dalje nastavljaju svoje izvrsavanje
class Sem {
public:
    static int sem_open(Sem** handle, unsigned init);
    static int sem_close(Sem* handle);
    static int sem_wait(Sem* id);
    static int sem_signal(Sem* id);
    static int sem_trywait(Sem* id);

private:
    Sem(int init) : val(init) {
//        blockedThreadsQueue = new List();
    }
    // static List<TCB> blockedThreadsQueue;
    List<TCB> blockedThreadsQueue;

    int val;
};


#endif //OS_PROJEKAT_SEM_HPP
