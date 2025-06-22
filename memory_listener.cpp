#include "memory_listener.h"
#include <cstdio>
#include <cstdlib>
#include <new>

MemoryAction* memoryActionsHead;

void* allocWithMemoryAction(std::size_t size, MemoryAction** action) {
    void* ptr = std::malloc(size + sizeof(MemoryAction));
    if (ptr == NULL) {
        throw std::bad_alloc{}; // TODO replace and put our own throw
    }
    char* actionLocation = static_cast<char*>(ptr) + size;                    // Char to allow exact pointer arithmetic.
    *action = static_cast<MemoryAction*>(static_cast<void*>(actionLocation)); // to cheat the compiler with casting.
    return ptr;
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
    //    std::puts("3) delete(void*)");
    std::free(ptr);
}

void operator delete(void* ptr, std::size_t size) noexcept {
    //    std::printf("4) delete(void*, size_t), size = %zu\n", size);
    std::free(ptr);
}

void operator delete[](void* ptr) noexcept {
    //    std::puts("5) delete[](void* ptr)");
    std::free(ptr);
}

void operator delete[](void* ptr, std::size_t size) noexcept {
    //    std::printf("6) delete[](void*, size_t), size = %zu\n", size);
    std::free(ptr);
}
