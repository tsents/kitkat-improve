#pragma once

#include <cstddef> // needed here for size_t.

enum class ActionTypes : char {
    NEW_SIZE,
    NEW_BRACKET,
    // DELETE_PTR,
    // DELETE_SIZE,
    // DELETE_BRACKET_PTR,
    // DELETE_BRACET_SIZE
};

/*
 * A list of "Memory actions" that were executed. each one
 * consisting of the action (which new? or delete?), the size
 * requested (if it exists for the function) and the prev action.
 */
class MemoryAction {
public:
    MemoryAction* m_prev;
    MemoryAction* m_next;
    ActionTypes m_action;
    std::size_t m_size;
    void printSummery();
};

/*
 * A function that wraps malloc, allowing allocation with the addition
 * of MemoryAction at the start. Additionaly, provides utilitis like throwing
 * on fail, returning the pointer of the location of the memory action, and
 * chaining him to the MemoryAction global list.
 *
 * size [IN] The original requested allocation size.
 * action [OUT] The location were the MemoryAction was alloced (is ptr + size).
 */
void* allocWithMemoryAction(std::size_t size, MemoryAction** action);

/*
 * A function that wraps free, to allow freeing of objects that have MemoryAction
 * allocated with them.
 * it makes sure to change the ptr to point to the true start of the object,
 * and update the MemoryAction golbal list.
 *
 * ptr [IN] The pointer to the start of the object, to free.
 */
void freeWithMemoryAction(void* ptr);

/*
 * Returns the location of the memory action based on the ptr.
 * usefull if we will change the location laster.
 *
 * ptr [IN] The pointer to the object.
 * return   The location of the MemoryAction assosiated with it.
 */
MemoryAction* getMemoryAction(void* ptr);

/*
 * The defualt new and delete operations. These are called
 * by any new and delete used in the program, including objects.
 *
 * For example, new S[10] will call new[](sizeof(10) * 10);
 * this DOESNT cover if some user adds additional overloads to the new operator.
 * like new(3) S[10], which will call new[] with an additional argument
 */
void* operator new(std::size_t size);
void* operator new[](std::size_t size);
void operator delete(void* ptr) noexcept;
void operator delete(void* ptr, std::size_t size) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete[](void* ptr, std::size_t size) noexcept;
