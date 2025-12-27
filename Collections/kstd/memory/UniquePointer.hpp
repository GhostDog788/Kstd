#pragma once
#include <stddef.h>

namespace kstd {

    //template <class T>
    //constexpr T&& move(T& t) noexcept { return static_cast<T&&>(t); }

    // -------------------- unique_ptr<T> --------------------
    template<class T>
    class unique_ptr
    {
    public:
        unique_ptr() noexcept = default;
        explicit unique_ptr(T* p) noexcept : m_ptr(p) {}

        unique_ptr(const unique_ptr&) = delete;
        unique_ptr& operator=(const unique_ptr&) = delete;

        unique_ptr(unique_ptr&& other) noexcept : m_ptr(other.m_ptr) {
            other.m_ptr = nullptr;
        }

        unique_ptr& operator=(unique_ptr&& other) noexcept {
            if (this != &other) {
                reset();
                m_ptr = other.m_ptr;
                other.m_ptr = nullptr;
            }
            return *this;
        }

        ~unique_ptr() noexcept { reset(); }

        T* get() const noexcept { return m_ptr; }
        explicit operator bool() const noexcept { return m_ptr != nullptr; }

        T& operator*() const noexcept { return *m_ptr; }
        T* operator->() const noexcept { return m_ptr; }

        T* release() noexcept {
            T* p = m_ptr;
            m_ptr = nullptr;
            return p;
        }

        void reset(T* p = nullptr) noexcept {
            if (m_ptr) {
                // This calls T::~T() and then your global operator delete
                delete m_ptr;
            }
            m_ptr = p;
        }

        void dismiss() noexcept { // no delete
            m_ptr = nullptr;
        }

    private:
        T* m_ptr = nullptr;
    };

    // -------------------- unique_ptr<T[]> --------------------
    template<class T>
    class unique_ptr<T[]>
    {
    public:
        unique_ptr() noexcept = default;
        explicit unique_ptr(T* p) noexcept : m_ptr(p) {}

        unique_ptr(const unique_ptr&) = delete;
        unique_ptr& operator=(const unique_ptr&) = delete;

        unique_ptr(unique_ptr&& other) noexcept : m_ptr(other.m_ptr) {
            other.m_ptr = nullptr;
        }

        unique_ptr& operator=(unique_ptr&& other) noexcept {
            if (this != &other) {
                reset();
                m_ptr = other.m_ptr;
                other.m_ptr = nullptr;
            }
            return *this;
        }

        ~unique_ptr() noexcept { reset(); }

        T* get() const noexcept { return m_ptr; }
        explicit operator bool() const noexcept { return m_ptr != nullptr; }

        T& operator[](size_t i) const noexcept { return m_ptr[i]; }

        T* release() noexcept {
            T* p = m_ptr;
            m_ptr = nullptr;
            return p;
        }

        void reset(T* p = nullptr) noexcept {
            if (m_ptr) {
                // Calls destructors for ALL elements then your global operator delete[]
                delete[] m_ptr;
            }
            m_ptr = p;
        }

        void dismiss() noexcept { // no delete
            m_ptr = nullptr;
        }

    private:
        T* m_ptr = nullptr;
    };

} // namespace kstd
