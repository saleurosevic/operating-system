//
// Created by os on 1/23/24.
//

#include "../h/syscall_c.hpp"


void *mem_alloc(size_t size) //size je u bajtovima
{
    if(size == 0) return nullptr;
    size_t mySize = 0;
    if(size % MEM_BLOCK_SIZE == 0)
        mySize = size / MEM_BLOCK_SIZE;
    else{
        mySize = size/MEM_BLOCK_SIZE + 1;
    }
    uint64 broj = 0x01;
    uint64 volatile returnValue = 0; //probaj dal moze ovde da bude tipa void*
    __asm__ volatile ("mv a1, %0" : : "r" (mySize));
    __asm__ volatile ("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    __asm__ volatile("mv %0, a0" : "=r" (returnValue));
    return (void*)returnValue;
}

int mem_free(void *addr)
{
    if(addr == nullptr) return -1;
    uint64 broj = 0x02;
    int volatile returnValue = 0;
    __asm__ volatile ("mv a1, %0" : : "r" (addr));
    __asm__ volatile ("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    __asm__ volatile("mv %0, a0" : "=r" (returnValue));
    //addr = nullptr;
    return  returnValue;
}

//------------------------------------------------------------------------

char getc()
{
    uint64 broj = 0x41;
    uint64 volatile returnValue = 0;
//    __asm__ volatile ("mv a1, %0" : : "r" (0));
    __asm__ volatile ("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %0, a0" : "=r" (returnValue));
    return (char)returnValue;
}

void putc(char c)
{
    uint64 broj = 0x42;
    __asm__ volatile ("mv a1, %0" : : "r" ((uint64)c));
    __asm__ volatile ("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
}

//------------------------------------------------------------------------

void changeToUserMode()
{
    uint64 broj = 0x05; //lupio broj
    __asm__ volatile ("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
}

//------------------------------------------------------------------------

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) //za main sve je null, samo handle = mainThread
{
    //alocira stek i pozove mi ecall za 0x11 i posle za 0x11 pozivamo createThread
    void volatile *stack;
    if(start_routine != nullptr){
        stack = mem_alloc(DEFAULT_STACK_SIZE); //alociramo stek za ovu velicinu, ovde smem da pozivam ovaj sistemski poziv
    } else{
        stack = nullptr;  //ovo je za main
    }
    uint64 broj = 0x11;
    int  volatile returnValue = 0;

    __asm__ volatile ("mv a6, %0" : : "r" ((uint64)stack)); //mozda ne radi a4 pa probaj a6 i vidi sta je on sa t2 i handle nesto eksperimentisao
    __asm__ volatile ("mv a3, %0" : : "r" ((uint64)arg));
    __asm__ volatile ("mv a2, %0" : : "r" ((uint64)start_routine));
    __asm__ volatile ("mv a1, %0" : : "r" ((uint64)handle));
    __asm__ volatile ("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall"); //prelazak u prekidnu rutinu - sistemski rezim
    __asm__ volatile ("mv %0, a0" : "=r" (returnValue));
    return returnValue;
}

int thread_exit()
{
    uint64 broj = 0x12;
    __asm__ volatile ("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    int volatile returnValue = 0;
    __asm__ volatile ("mv %0, a0" : "=r" (returnValue));
    return  returnValue;
}

void thread_dispatch()
{
    uint64 broj = 0x13;
    __asm__ volatile ("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
}

void thread_join(thread_t handle)
{
    uint64 broj = 0x14;
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");
    int i = 0;
    i = i+5;
}

//------------------------------------------------------------------------

int sem_open(sem_t *handle, unsigned init)
{
    uint64 returnValue;
    uint64 broj = 0x21;
    __asm__ volatile ("mv a2, %0" : : "r" ((uint64)init));
    __asm__ volatile ("mv a1, %0" : : "r" ((uint64)handle));
    __asm__ volatile ("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");

    __asm__ volatile ("mv %0, a0" : "=r" (returnValue));
    return  returnValue;
}

int sem_close (sem_t handle)
{
    uint64 returnValue;
    uint64 broj = 0x22;
    __asm__ volatile ("mv a1, %0" : : "r" ((uint64)handle));
    __asm__ volatile ("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");

    __asm__ volatile ("mv %0, a0" : "=r" (returnValue));
    return  returnValue;
}

int sem_wait (sem_t id)
{
    uint64 returnValue;
    uint64 broj = 0x23;
    __asm__ volatile ("mv a1, %0" : : "r" ((uint64)id));
    __asm__ volatile ("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");

    __asm__ volatile ("mv %0, a0" : "=r" (returnValue));
    return  returnValue;
}

int sem_signal (sem_t id)
{
    uint64 returnValue;
    uint64 broj = 0x24;
    __asm__ volatile ("mv a1, %0" : : "r" ((uint64)id));
    __asm__ volatile ("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");

    __asm__ volatile ("mv %0, a0" : "=r" (returnValue));
    return  returnValue;
}

int sem_trywait (sem_t id)
{
    uint64 returnValue;
    uint64 broj = 0x26;
    __asm__ volatile ("mv a1, %0" : : "r" ((uint64)id));
    __asm__ volatile ("mv a0, %0" : : "r" (broj));
    __asm__ volatile ("ecall");

    __asm__ volatile ("mv %0, a0" : "=r" (returnValue));
    return  returnValue;
}