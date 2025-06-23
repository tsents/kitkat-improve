#include "memory_listener.h"

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
    (*action)->m_size = size; // size is the original size.
    (*action)->m_location = newLocation;
    return static_cast<void*>(newLocation);
}

void freeWithMemoryAction(void* ptr) {
    // Changes the ptr to the true obeject start. with the memoryAction.
    char* newLocation = static_cast<char*>(ptr) - sizeof(MemoryAction);
    MemoryAction* action = getMemoryAction(ptr);
    if (action->m_prev != NULL) {
        action->m_prev->m_next = action->m_next;
    }
    if (action->m_next != NULL) {
        action->m_next->m_prev = action->m_prev;
    }
    free(newLocation);
}

MemoryAction* getMemoryAction(void* ptr) {
    char* newLocation = static_cast<char*>(ptr) - sizeof(MemoryAction);
    MemoryAction* action = static_cast<MemoryAction*>(static_cast<void*>(newLocation));
    return action;
}

void MemoryAction::printSummery(std::ostream& stream) {
    stream << "Action ";
    switch (this->m_action) {
    case ActionTypes::NEW_SIZE:
        stream << "new ";
        break;
    case ActionTypes::NEW_BRACKET:
        stream << "new[] ";
        break;
    }
    stream << "size = " << this->m_size << " located at " << this->m_location;
}

void MemoryAction::printListSummery() {
    std::cout << "Current allocation state (in reverse order)" << std::endl;
    std::cout << ">>>>>>>>>" << std::endl;
    for (MemoryAction* listRunner = memoryActionsHead; listRunner != NULL; listRunner = listRunner->m_prev) {
        listRunner->printSummery(std::cout);
        std::cout << std::endl;
    }
    std::cout << "<<<<<<<<<" << std::endl;
}

void* operator new(std::size_t size) {
    MemoryAction* action;
    void* ptr = allocWithMemoryAction(size, &action);
    action->m_action = ActionTypes::NEW_SIZE;
    action->printSummery(std::cerr);
    std::cerr << std::endl;
    return ptr;
}

void* operator new[](std::size_t size) {
    MemoryAction* action;
    void* ptr = allocWithMemoryAction(size, &action);
    action->m_action = ActionTypes::NEW_BRACKET;
    action->printSummery(std::cerr);
    std::cerr << std::endl;
    return ptr;
}

void operator delete(void* ptr) noexcept {
    MemoryAction* action = getMemoryAction(ptr);
    action->printSummery(std::cerr);
    freeWithMemoryAction(ptr);
}

void operator delete(void* ptr, std::size_t size) noexcept {
    MemoryAction* action = getMemoryAction(ptr);
    action->printSummery(std::cerr);
    std::cerr << " deleted by delete(void*, size_t)" << std::endl;
    freeWithMemoryAction(ptr);
}

void operator delete[](void* ptr) noexcept {
    MemoryAction* action = getMemoryAction(ptr);
    action->printSummery(std::cerr);
    std::cerr << " deleted by delete[](void*)" << std::endl;
    freeWithMemoryAction(ptr);
}

void operator delete[](void* ptr, std::size_t size) noexcept {
    MemoryAction* action = getMemoryAction(ptr);
    action->printSummery(std::cerr);
    std::cerr << " deleted by delete[](void*, size_t)" << std::endl;
    freeWithMemoryAction(ptr);
}
