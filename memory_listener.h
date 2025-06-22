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
 *
 * The reason for struct instead of class is that we won't be able
 * to case new! thus we won't be able to have a constructor and deconstructor,
 * and this class just presents a data scheme.
 */
struct MemoryAction {
    const MemoryAction* m_prev;
    ActionTypes m_action;
    std::size_t m_size;
};

/*
 * A function that wraps malloc, allowing allocation with the addition
 * of MemoryAction at the end. Additionaly, provides utilitis like throwing
 * on fail, returning the pointer of the location of the memory action, and
 * chaining him to the MemoryAction global list.
 *
 * size [IN] The original requested allocation size.
 * action [OUT] The location were the MemoryAction was alloced (is ptr + size).
 */
void* allocWithMemoryAction(std::size_t size, MemoryAction** action);

void* operator new(std::size_t size);
void* operator new[](std::size_t size);
void operator delete(void* ptr) noexcept;
void operator delete(void* ptr, std::size_t size) noexcept;

void operator delete[](void* ptr) noexcept;
void operator delete[](void* ptr, std::size_t size) noexcept;
