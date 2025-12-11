//
// Created by os on 1/23/24.
//

#include "../h/MemoryAllocator.hpp"

MemoryAllocator::MemDesc* MemoryAllocator::memHead = nullptr;

void MemoryAllocator::mem_init()
{
    memHead = (MemDesc*) HEAP_START_ADDR; //odavde pocinje, void* moze u bilo sta konverzija
    MemDesc *pom = memHead;
    //jedan blok mi je za strukturu MemDesc a ostatak tj size za smestanje sadrzaja
    //pom->size = (size_t)HEAP_END_ADDR  - (size_t)HEAP_START_ADDR - MEM_BLOCK_SIZE; //u bajtovima isto je jer size_t vraca apsolutnu adresu iz memorije
    pom->size = (size_t) ((char *) HEAP_END_ADDR - (char *) HEAP_START_ADDR - MEM_BLOCK_SIZE); //u bajtovima
    pom->next = nullptr;
    pom->prev = nullptr;
    pom->freeDesc = true;
}

void* MemoryAllocator::mem_alloc(size_t size) //size - broj blokova
{
    //jedan blok mi ide za smestanje strukture i jos jedan za ovaj sadrzaj size
    if(size == 0) return nullptr;

    size_t mySize = size * MEM_BLOCK_SIZE; //size mi je broj blokova i ovde pretvaram u bajtove
//    if(size % MEM_BLOCK_SIZE == 0)
//        mySize = size;
//    else{
//        mySize = (size/MEM_BLOCK_SIZE + 1) * MEM_BLOCK_SIZE;
//    }

    MemDesc *tmp = memHead;
    while (tmp != nullptr){
        if(tmp->size >= mySize && tmp->freeDesc)
            break;
        tmp = tmp->next;
    }
    if(tmp == nullptr) return nullptr; //nema mesta u memoriji

    //nasli smo mesto u memoriji
    size_t remainingSize = tmp->size - mySize;
    if(remainingSize >= 2 * MEM_BLOCK_SIZE){ //dal uzimamo ceo ili moze da se smesti novi deksriptor
        tmp->size = mySize;
        tmp->freeDesc = false;
        MemDesc *newBlk = (MemDesc*)((char*)tmp + MEM_BLOCK_SIZE + mySize); //mora ovako posto nema new
        newBlk->prev = tmp;
        newBlk->next = tmp->next;
        newBlk->size = remainingSize - MEM_BLOCK_SIZE;
        newBlk->freeDesc = true;
        tmp->next = newBlk;
    }
    else{ //zauzimamo ceo ovaj blok iako je malo veci nego sto nam se trazi
        tmp->freeDesc = false;
    }

    return (char*)tmp + MEM_BLOCK_SIZE;
}


//oslobadja delove memorije koji su ranije alocirani sa mem_alloc - ne mogu da izbacujem taj deo iz liste jer tesko da ga vratim posle
int MemoryAllocator::mem_free(void *addr)
{
    if(addr == nullptr) return -1;
    MemDesc *tmp = memHead;
    while(((char*)tmp + MEM_BLOCK_SIZE) != (char*)addr){
        tmp = tmp->next;
        if(tmp == nullptr) break;
    }
    if(tmp == nullptr) return -1;
    if(tmp->freeDesc) return -1;
    tmp->freeDesc = true;

    //spojimo ih u jedan ako su ovi pre i posle njega isto free
    if(tmp->next != nullptr) {
        if(tmp->next->freeDesc) {
            tmp->size = tmp->size + MEM_BLOCK_SIZE + tmp->next->size;
            tmp->next = tmp->next->next;
            if(tmp->next != nullptr)
                tmp->next->prev = tmp;
        }
    }
    if(tmp->prev != nullptr) {
        if(tmp->prev->freeDesc){
            MemDesc *prevTmp = tmp->prev;
            prevTmp->size = prevTmp->size + MEM_BLOCK_SIZE + tmp->size;
            prevTmp->next = tmp->next;
            prevTmp->freeDesc = true;
            if(tmp->next != nullptr)
                tmp->next->prev = prevTmp;
        }
    }
    return 0;
}
