//
// Created by os on 1/23/24.
//

#ifndef OS_PROJEKAT_MEMORYALLOCATOR_HPP
#define OS_PROJEKAT_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

//vodimo evidenciju samo slobodnih blokova
//svaki slobodni deo ima zaglavlje u kome kaze koja mu je velicina kao i pokazivace na naredni i prethodni slobodni deo

class MemoryAllocator {
public:
    static void *mem_alloc(size_t size); //alocira size bajtova, zaokruzeno i poravnato na blokove velicine MEM_BLOCK_SIZE
    static int mem_free(void* addr); //oslobadja prostor prethodno zauzet pomocu mem_alloc
    static void mem_init();

private:
    struct MemDesc
    {
        size_t size; //velicina bez zaglavlja
        MemDesc *next;
        MemDesc *prev;
        bool freeDesc;
    };
    static MemDesc* memHead; //lista slobodnih fragmenata
};


#endif //OS_PROJEKAT_MEMORYALLOCATOR_HPP
