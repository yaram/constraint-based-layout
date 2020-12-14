#pragma once

[[noreturn]] void panic();

void *allocate(size_t size);

void *reallocate(void *pointer, size_t new_size);

void deallocate(void *pointer);