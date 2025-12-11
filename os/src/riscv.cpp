//
// Created by os on 1/23/24.
//

#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/printing.hpp"
#include "../lib/console.h"
#include "../h/tcb.hpp"
#include "../h/sem.hpp"

void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra"); //u sepc koje kupi sret uneli ra ove nase niti
    // ms_sstatus(SSTATUS_SPP); //ovo mi je za test 7
    __asm__ volatile("sret"); //ovo sigurno popuje spp, kao i ceo sstatus
}

void Riscv::handleSupervisorTrap()
{
    uint64 broj;
    __asm__ volatile("mv %0, a0" : "=r" (broj));

    uint64 param1, param2, param3, param4;
    __asm__ volatile("mv %0, a1" : "=r" (param1));
    __asm__ volatile("mv %0, a2" : "=r" (param2));
    __asm__ volatile("mv %0, a3" : "=r" (param3));
    __asm__ volatile("mv %0, a6" : "=r" (param4));

    uint64 ret = 0;
    TCB* tcb;
    uint64 scause = r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        uint64 sepc = r_sepc() + 4;
        uint64 sstatus = r_sstatus();


//        uint64 clearedSstatus;

        switch (broj) {
            //mem_alloc
            case 0x01:
                ret = (uint64) MemoryAllocator::mem_alloc((uint64) param1); //
                __asm__ volatile ("mv a0, %0" : : "r" (ret));
                break;

            //mem_free
            case 0x02:
                ret = (uint64) MemoryAllocator::mem_free((void *) param1);
                __asm__ volatile("mv a0, %0" : : "r" (ret)); //povratna vrednost ide u a0
                break;

            //putc
            case 0x42:
                __putc((char) param1);
                break;

            //getc
            case 0x41:
                broj= __getc();
                __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(broj));
                break;

            //change to user mode
            case 0x05:
                // sstatus = sstatus & (0xFFFFFFFFFFFFFFFFUL - 256);
                mc_sstatus(SSTATUS_SPP);
                // sstatus = r_sstatus();
                break;

            //thread_create
            case 0x11:
                ret = TCB::createThread((thread_t *) param1, (void(*)(void *)) param2, (void *)param3, (uint64*) param4); //handle, start routine, arg, stack
                __asm__ volatile ("mv a0, %0" : : "r" (ret));
                break;

            //thread_exit
            case 0x12:
                ret = TCB::exit();
                __asm__ volatile ("mv a0, %0" : : "r" (ret));
                break;

            //thread_dispatch
            case 0x13:
                TCB::dispatch();
                break;

            //thread_join
            case 0x14:
                tcb = (thread_t)param1;
                TCB::thread_join(tcb);
                break;

            //sem_open
            case 0x21:
                ret = Sem::sem_open((sem_t*)param1, (int)param2);
                __asm__ volatile ("mv a0, %0" : : "r" (ret));
                break;

            //sem_close
            case 0x22:
                ret = Sem::sem_close((sem_t) param1);
                __asm__ volatile ("mv a0, %0" : : "r" (ret));
                break;

            //sem_wait
            case 0x23:
                ret = Sem::sem_wait((sem_t) param1);
                __asm__ volatile ("mv a0, %0" : : "r" (ret));
                break;

            //sem_signal
            case 0x24:
                ret = Sem::sem_signal((sem_t) param1);
                __asm__ volatile ("mv a0, %0" : : "r" (ret));
                break;

            //sem_trywait
            case 0x26:
                ret = Sem::sem_trywait((sem_t) param1);
                __asm__ volatile ("mv a0, %0" : : "r" (ret));
                break;

            default:
                break;
        }

        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000009UL) //console
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        uint64 volatile sepc = r_sepc(); //u sepc je povratna adresa nije u ra
        uint64 volatile sstatus = r_sstatus();
        console_handler();
        w_sstatus(sstatus); //sepc i sstatus su lokalne promenljive i svaka nit ih ima za sebe i onda ovde uzimamo te vrednosti od ove niti sto se sad izvrsava i upisujemo ih u registe
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL) //timer
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
    }
    else {
        // __asm __volatile("csrc sip, 0x02");

        //unexpected trap
        printString("");
        uint64 scause = r_scause();
        printString("scause: ");
        printInt((int)scause);
        printString("\n");

        uint64 pc = r_sepc();
        printString("pc: ");
        printInt((int)pc);
        printString("\n");

        uint64 stval = r_stval();
        printString("stval: ");
        printInt((int)stval);
        printString("\n");

        while(true);
    }
}

// void Riscv::handleTimerInterrupt()
// {
//     // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
//     // software interrupt pending,
//     mc_sip(SIP_SSIP); // SSIP - 1 ima zahtev za softverskim prekidom (timer) i mi samo ovde postavljamo to na 0 i kazemo da se obradio prekid
// }
//
// void Riscv::handleConsoleInterrupt()
// {
//     console_handler();
// }
//
// void Riscv::unexpectedTrap()
// {
//     uint64 scause = r_scause();
//     printString("scause: ");
//     printInt((int)scause);
//     printString("\n");
//
//     uint64 pc = r_sepc();
//     printString("pc: ");
//     printInt((int)pc);
//     printString("\n");
//
//     uint64 stval = r_stval();
//     printString("stval: ");
//     printInt((int)stval);
//     printString("\n");
// }