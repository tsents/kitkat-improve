#include "memory_listener.h"
#include <cstdio>
#include <cstdlib>
#include <new>

MemoryAction* memoryActionsHead;

void* allocWithMemoryAction(std::size_t size, MemoryAction** action) {
    void* ptr = std::malloc(size + sizeof(MemoryAction));
    if (ptr == NULL) {
        throw std::bad_alloc{};
    }
    char* newLocation = static_cast<char*>(ptr) + sizeof(MemoryAction); // puts memoryAction behind the actual pointer.
    *action = static_cast<MemoryAction*>(ptr);                          // to cheat the compiler with casting.

    // Update the linked list
    (*action)->m_prev = memoryActionsHead;
    if (memoryActionsHead != NULL) {
        memoryActionsHead->m_next = (*action);
    }
    memoryActionsHead = (*action);

    return static_cast<void*>(newLocation);
}

void freeWithMemoryAction(void* ptr) {
    char* newLocation = static_cast<char*>(ptr) -
                        sizeof(MemoryAction); // Changes the ptr to the true obeject start. with the memoryAction.
    MemoryAction* action = static_cast<MemoryAction*>(static_cast<void*>(newLocation));
    if (action->m_prev != NULL) {
        action->m_prev->m_next = action->m_next;
    }
    if (action->m_next != NULL) {
        action->m_next->m_prev = action->m_prev;
    }
    free(newLocation);
}

void* operator new(std::size_t size) {
    std::printf("1) new(size_t), size = %zu\n", size);
    MemoryAction* action;
    void* ptr = allocWithMemoryAction(size, &action);
    return ptr;
}

void* operator new[](std::size_t size) {
    std::printf("2) new[](size_t), size = %zu\n", size);
    MemoryAction* action;
    void* ptr = allocWithMemoryAction(size, &action);
    return ptr;
}

void operator delete(void* ptr) noexcept {
    std::puts("3) delete(void*)");
    freeWithMemoryAction(ptr);
}

void operator delete(void* ptr, std::size_t size) noexcept {
    std::printf("4) delete(void*, size_t), size = %zu\n", size);
    freeWithMemoryAction(ptr);
}

void operator delete[](void* ptr) noexcept {
    std::puts("5) delete[](void* ptr)");
    freeWithMemoryAction(ptr);
}

void operator delete[](void* ptr, std::size_t size) noexcept {
    std::printf("6) delete[](void*, size_t), size = %zu\n", size);
    freeWithMemoryAction(ptr);
}
