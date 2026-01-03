#pragma once

#ifdef _KERNEL_MODE
#include <ntddk.h>

namespace kstd
{
    struct nothrow_t { explicit constexpr nothrow_t() = default; };
    inline constexpr nothrow_t nothrow{};

#if defined(__cpp_aligned_new) && (__cpp_aligned_new >= 201606L)
    enum class align_val_t : SIZE_T {};
#endif
}

#ifndef KSTD_NEW_TAG
#define KSTD_NEW_TAG 'Kstd'
#endif

#ifndef KSTD_NEW_FLAGS
#define KSTD_NEW_FLAGS (POOL_FLAG_NON_PAGED | POOL_FLAG_UNINITIALIZED)
#endif

#ifndef KSTD_NEW_NO_RAISE
#define KSTD_NEW_NO_RAISE 0
#endif

inline void* operator new(SIZE_T, void* p) noexcept { return p; }
inline void  operator delete(void*, void*) noexcept {}

namespace kstd::detail
{
    __forceinline void* alloc_raw(SIZE_T size) noexcept
    {
        if (size == 0) size = 1;
        return ExAllocatePool2((POOL_FLAGS)KSTD_NEW_FLAGS, size, (ULONG)KSTD_NEW_TAG);
    }

    __forceinline void free_raw(void* p) noexcept
    {
        if (p) ExFreePool(p);
    }

    __forceinline void* alloc_or_raise(SIZE_T size)
    {
        void* p = alloc_raw(size);
        if (!p)
        {
#if KSTD_NEW_NO_RAISE
            return nullptr;
#else
            ExRaiseStatus(STATUS_INSUFFICIENT_RESOURCES);
#endif
        }
        return p;
    }
}

void* __cdecl operator new(SIZE_T size)
{
    return kstd::detail::alloc_or_raise(size);
}

void* __cdecl operator new[](SIZE_T size)
{
    return kstd::detail::alloc_or_raise(size);
}

void* __cdecl operator new(SIZE_T size, const kstd::nothrow_t&) noexcept
{
    return kstd::detail::alloc_raw(size);
}

void* __cdecl operator new[](SIZE_T size, const kstd::nothrow_t&) noexcept
{
    return kstd::detail::alloc_raw(size);
}

void __cdecl operator delete(void* p) noexcept
{
    kstd::detail::free_raw(p);
}

void __cdecl operator delete[](void* p) noexcept
{
    kstd::detail::free_raw(p);
}

void __cdecl operator delete(void* p, SIZE_T) noexcept
{
    kstd::detail::free_raw(p);
}

void __cdecl operator delete[](void* p, SIZE_T) noexcept
{
    kstd::detail::free_raw(p);
}

#if defined(__cpp_aligned_new) && (__cpp_aligned_new >= 201606L)
inline void* __cdecl operator new(SIZE_T size, kstd::align_val_t)
{
    return kstd::detail::alloc_or_raise(size);
}

inline void* __cdecl operator new[](SIZE_T size, kstd::align_val_t)
{
    return kstd::detail::alloc_or_raise(size);
}

inline void* __cdecl operator new(SIZE_T size, kstd::align_val_t, const kstd::nothrow_t&) noexcept
{
    return kstd::detail::alloc_raw(size);
}

inline void* __cdecl operator new[](SIZE_T size, kstd::align_val_t, const kstd::nothrow_t&) noexcept
{
    return kstd::detail::alloc_raw(size);
}

inline void __cdecl operator delete(void* p, kstd::align_val_t) noexcept
{
    kstd::detail::free_raw(p);
}

inline void __cdecl operator delete[](void* p, kstd::align_val_t) noexcept
{
    kstd::detail::free_raw(p);
}

inline void __cdecl operator delete(void* p, SIZE_T, kstd::align_val_t) noexcept
{
    kstd::detail::free_raw(p);
}

inline void __cdecl operator delete[](void* p, SIZE_T, kstd::align_val_t) noexcept
{
    kstd::detail::free_raw(p);
}
#endif
#endif