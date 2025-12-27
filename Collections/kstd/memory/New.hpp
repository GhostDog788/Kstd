#pragma once

#ifdef _KERNEL_MODE
#include <ntddk.h>
#else
#include <cstdlib>  // malloc/free
#include <stddef.h>
#include <new> // std::nothrow_t
#endif

namespace kstd {

    // ---------- Cross-mode configuration ----------
#ifdef _KERNEL_MODE
    constexpr POOL_TYPE kDefaultPool = NonPagedPoolNx;
    constexpr unsigned long kDefaultTag = 'dtsK';
#else
    constexpr unsigned long kDefaultTag = 0; // unused in user mode
#endif

    // Tag type: new (kstd::pool_tag<'TAG1'>) T(...)
    template<unsigned long TagV>
    struct pool_tag_t { static constexpr unsigned long value = TagV; };

    template<unsigned long TagV>
    inline constexpr pool_tag_t<TagV> pool_tag{};

#ifdef _KERNEL_MODE
    // Pool type wrapper: new (kstd::pool_type{PagedPool}) T(...)
    struct pool_type_t { POOL_TYPE type; };
    constexpr pool_type_t paged_pool{ PagedPool };
    constexpr pool_type_t nonpaged_pool{ NonPagedPoolNx };
#else
    // User mode: keep same API, but pool type is ignored
    struct pool_type_t { int ignored; };
    constexpr pool_type_t paged_pool{ 0 };
    constexpr pool_type_t nonpaged_pool{ 0 };
#endif

    // ---------- Allocation backend ----------
    inline void* alloc_bytes(size_t sz,
#ifdef _KERNEL_MODE
        POOL_TYPE pool,
        unsigned long tag
#else
        int /*pool*/,
        unsigned long /*tag*/
#endif
    ) noexcept
    {
        if (sz == 0) sz = 1;

#ifdef _KERNEL_MODE
        return ExAllocatePool2(pool, sz, tag);
#else
        return std::malloc(sz);
#endif
    }

    inline void free_bytes(void* p) noexcept
    {
        if (!p) return;
#ifdef _KERNEL_MODE
        ExFreePool(p);
#else
        std::free(p);
#endif
    }

} // namespace kstd

// ============================================================
// Global operator new/delete (single object)
// ============================================================

inline void* __cdecl operator new(size_t, void* p) noexcept { return p; }

void* __cdecl operator new(size_t sz)
{
#ifdef _KERNEL_MODE
    return kstd::alloc_bytes(sz, kstd::kDefaultPool, kstd::kDefaultTag);
#else
    return kstd::alloc_bytes(sz, 0, 0);
#endif
}

inline void __cdecl operator delete(void*, void*) noexcept {}

void __cdecl operator delete(void* p) noexcept
{
    kstd::free_bytes(p);
}

// sized delete (MSVC may emit)
void __cdecl operator delete(void* p, size_t) noexcept
{
    kstd::free_bytes(p);
}

#ifndef _KERNEL_MODE
void* __cdecl operator new(size_t sz, const std::nothrow_t&) noexcept
{
    return ::operator new(sz);
}

void __cdecl operator delete(void* p, const std::nothrow_t&) noexcept
{
    ::operator delete(p);
}
#endif

// ============================================================
// Global operator new/delete (arrays)
// ============================================================

inline void* __cdecl operator new[](size_t, void* p) noexcept { return p; }

void* __cdecl operator new[](size_t sz)
{
#ifdef _KERNEL_MODE
    return kstd::alloc_bytes(sz, kstd::kDefaultPool, kstd::kDefaultTag);
#else
    return kstd::alloc_bytes(sz, 0, 0);
#endif
}

inline void __cdecl operator delete[](void*, void*) noexcept {}

void __cdecl operator delete[](void* p) noexcept
{
    kstd::free_bytes(p);
}

void __cdecl operator delete[](void* p, size_t) noexcept
{
    kstd::free_bytes(p);
}

#ifndef _KERNEL_MODE
void* __cdecl operator new[](size_t sz, const std::nothrow_t&) noexcept
{
    return ::operator new[](sz);
}

void __cdecl operator delete[](void* p, const std::nothrow_t&) noexcept
{
    ::operator delete[](p);
}
#endif

// ============================================================
// Tagged / pool-selecting placement new (cross-mode)
// ============================================================

// new (pool_tag<'TAG1'>) T
template<unsigned long TagV>
void* __cdecl operator new(size_t sz, kstd::pool_tag_t<TagV>) noexcept
{
#ifdef _KERNEL_MODE
    return kstd::alloc_bytes(sz, kstd::kDefaultPool, TagV);
#else
    // user mode ignores tag, but API stays the same
    return kstd::alloc_bytes(sz, 0, 0);
#endif
}

// new (pool_type_t{...}) T
inline void* __cdecl operator new(size_t sz, kstd::pool_type_t pt) noexcept
{
#ifdef _KERNEL_MODE
    return kstd::alloc_bytes(sz, pt.type, kstd::kDefaultTag);
#else
    (void)pt;
    return kstd::alloc_bytes(sz, 0, 0);
#endif
}

// new (pool_type_t{...}, pool_tag<'TAG1'>) T
template<unsigned long TagV>
inline void* __cdecl operator new(size_t sz, kstd::pool_type_t pt, kstd::pool_tag_t<TagV>) noexcept
{
#ifdef _KERNEL_MODE
    return kstd::alloc_bytes(sz, pt.type, TagV);
#else
    (void)pt;
    return kstd::alloc_bytes(sz, 0, 0);
#endif
}

// Matching placement deletes (only used if ctor throws; still define them)
template<unsigned long TagV>
inline void __cdecl operator delete(void* p, kstd::pool_tag_t<TagV>) noexcept
{
    kstd::free_bytes(p);
}

inline void __cdecl operator delete(void* p, kstd::pool_type_t) noexcept
{
    kstd::free_bytes(p);
}

template<unsigned long TagV>
inline void __cdecl operator delete(void* p, kstd::pool_type_t, kstd::pool_tag_t<TagV>) noexcept
{
    kstd::free_bytes(p);
}
