#pragma once

#include <cstdlib> // needed here for size_t.


void* operator new(std::size_t sz);
void* operator new[](std::size_t sz);
void operator delete(void* ptr) noexcept;
void operator delete(void* ptr, std::size_t size) noexcept;
 
void operator delete[](void* ptr) noexcept;
void operator delete[](void* ptr, std::size_t size) noexcept;
