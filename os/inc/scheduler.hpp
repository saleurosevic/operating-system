//
// Created by os on 1/25/24.
//

#ifndef OS_PROJEKAT_SCHEDULER_HPP
#define OS_PROJEKAT_SCHEDULER_HPP


#include "list.hpp"

class TCB;

class Scheduler
{
private:
    static List<TCB> readyThreadQueue; //static clan deljen izmedju svih objekata i moze mu se pristupiti direktno preko imena klase bez kreiranja objekta

public:
    static TCB *get();

    static void put(TCB *ccb);

};


#endif //OS_PROJEKAT_SCHEDULER_HPP
