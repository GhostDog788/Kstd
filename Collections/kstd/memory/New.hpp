#pragma once

#include <cstddef>
#ifdef _KERNEL_MODE
#include <ntddk.h>
#else
#include <cstdlib>
#endif

void* __cdecl operator new(size_t size)
{
#ifdef _KERNEL_MODE
	return ExAllocatePool2(POOL_FLAG_NON_PAGED, size, 'DTSK'); // 'KSTD' in little endian
#else
	return std::malloc(size);
#endif
}

void* __cdecl operator new(size_t size, void* place) noexcept
{
#ifdef _KERNEL_MODE
    UNREFERENCED_PARAMETER(size);
#endif
    return place; // placement new
}

void operator delete(void*, void*) noexcept {}
void __cdecl operator delete(void* ptr) noexcept
{
#ifdef _KERNEL_MODE
    ExFreePool(ptr);
#else
	std::free(ptr);
#endif
}

void __cdecl operator delete(void* ptr, size_t) noexcept
{
#ifdef _KERNEL_MODE
    ExFreePool(ptr);
#else
    std::free(ptr);
#endif
}
