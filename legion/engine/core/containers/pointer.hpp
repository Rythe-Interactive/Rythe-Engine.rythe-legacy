#pragma once
#include <core/platform/platform.hpp>
#include <core/types/primitives.hpp>

/**
 * @file pointer.hpp
 */

namespace legion::core
{
    /**@class pointer
     * @brief Non owning pointer wrapper.
     * @tparam T Type of the underlying pointer.
     */
    template<typename T>
    struct pointer;

    template<typename T>
    struct pointer<const T>
    {
        const T* ptr;

        L_NODISCARD constexpr const T* operator->() const noexcept { return ptr; }

        L_NODISCARD constexpr const T& operator*() const noexcept { return *ptr; }

        L_NODISCARD constexpr operator bool() const noexcept { return ptr; }
        L_NODISCARD constexpr operator const T* () const noexcept { return ptr; }

        constexpr pointer& operator=(const T* src) noexcept { ptr = src; return *this; }

        L_NODISCARD constexpr bool operator==(const pointer& other) const noexcept { return ptr == other.ptr; }
        L_NODISCARD constexpr bool operator!=(const pointer& other) const noexcept { return ptr != other.ptr; }
        L_NODISCARD constexpr bool operator< (const pointer& other) const noexcept { return ptr < other.ptr; }
        L_NODISCARD constexpr bool operator<=(const pointer& other) const noexcept { return ptr <= other.ptr; }
        L_NODISCARD constexpr bool operator> (const pointer& other) const noexcept { return ptr > other.ptr; }
        L_NODISCARD constexpr bool operator>=(const pointer& other) const noexcept { return ptr >= other.ptr; }

        L_NODISCARD constexpr bool operator==(const T* other) const noexcept { return ptr == other; }
        L_NODISCARD constexpr bool operator!=(const T* other) const noexcept { return ptr != other; }
        L_NODISCARD constexpr bool operator< (const T* other) const noexcept { return ptr < other; }
        L_NODISCARD constexpr bool operator<=(const T* other) const noexcept { return ptr <= other; }
        L_NODISCARD constexpr bool operator> (const T* other) const noexcept { return ptr > other; }
        L_NODISCARD constexpr bool operator>=(const T* other) const noexcept { return ptr >= other; }

        L_NODISCARD constexpr bool operator==(std::nullptr_t) const noexcept { return ptr == nullptr; }
        L_NODISCARD constexpr bool operator!=(std::nullptr_t) const noexcept { return ptr != nullptr; }

        constexpr pointer& operator++() noexcept { ptr++; return *this; }
        constexpr pointer& operator--() noexcept { ptr--; return *this; }
        constexpr pointer operator++(int) noexcept { return { ++ptr }; }
        constexpr pointer operator--(int) noexcept { return { --ptr }; }
        L_NODISCARD constexpr pointer operator+(diff_type n) const noexcept { return { ptr + n }; }
        L_NODISCARD constexpr pointer operator-(diff_type n) const noexcept { return { ptr - n }; }
        constexpr pointer& operator+=(diff_type n) noexcept { ptr += n; return *this; }
        constexpr pointer& operator-=(diff_type n) noexcept { ptr -= n; return *this; }
    };

    template<typename T>
    struct pointer
    {
        T* ptr;

        L_NODISCARD constexpr T* operator->() noexcept { return ptr; }
        L_NODISCARD constexpr const T* operator->() const noexcept { return ptr; }

        L_NODISCARD constexpr T& operator*() noexcept { return *ptr; }
        L_NODISCARD constexpr const T& operator*() const noexcept { return *ptr; }

        L_NODISCARD constexpr operator bool() const noexcept { return ptr; }
        L_NODISCARD constexpr operator const T* () const noexcept { return ptr; }
        L_NODISCARD constexpr operator T* () noexcept { return ptr; }
        L_NODISCARD constexpr operator pointer<const T>() noexcept { return { ptr }; }

        constexpr pointer& operator=(T* src) noexcept { ptr = src; return *this; }

        L_NODISCARD constexpr bool operator==(const pointer& other) const noexcept { return ptr == other.ptr; }
        L_NODISCARD constexpr bool operator!=(const pointer& other) const noexcept { return ptr != other.ptr; }
        L_NODISCARD constexpr bool operator< (const pointer& other) const noexcept { return ptr < other.ptr; }
        L_NODISCARD constexpr bool operator<=(const pointer& other) const noexcept { return ptr <= other.ptr; }
        L_NODISCARD constexpr bool operator> (const pointer& other) const noexcept { return ptr > other.ptr; }
        L_NODISCARD constexpr bool operator>=(const pointer& other) const noexcept { return ptr >= other.ptr; }

        L_NODISCARD constexpr bool operator==(const T* other) const noexcept { return ptr == other; }
        L_NODISCARD constexpr bool operator!=(const T* other) const noexcept { return ptr != other; }
        L_NODISCARD constexpr bool operator< (const T* other) const noexcept { return ptr < other; }
        L_NODISCARD constexpr bool operator<=(const T* other) const noexcept { return ptr <= other; }
        L_NODISCARD constexpr bool operator> (const T* other) const noexcept { return ptr > other; }
        L_NODISCARD constexpr bool operator>=(const T* other) const noexcept { return ptr >= other; }

        L_NODISCARD constexpr bool operator==(std::nullptr_t) const noexcept { return ptr == nullptr; }
        L_NODISCARD constexpr bool operator!=(std::nullptr_t) const noexcept { return ptr != nullptr; }

        constexpr pointer& operator++() noexcept { ptr++; return *this; }
        constexpr pointer& operator--() noexcept { ptr--; return *this; }
        constexpr pointer operator++(int) noexcept { return { ++ptr }; }
        constexpr pointer operator--(int) noexcept { return { --ptr }; }
        L_NODISCARD constexpr pointer operator+(diff_type n) const noexcept { return { ptr + n }; }
        L_NODISCARD constexpr pointer operator-(diff_type n) const noexcept { return { ptr - n }; }
        constexpr pointer& operator+=(diff_type n) noexcept { ptr += n; return *this; }
        constexpr pointer& operator-=(diff_type n) noexcept { ptr -= n; return *this; }
    };

    template<>
    struct pointer<const void>
    {
        const void* ptr;

        L_NODISCARD constexpr operator bool() const noexcept { return ptr; }
        L_NODISCARD constexpr operator const void* () const noexcept { return ptr; }

        constexpr pointer& operator=(const void* src) noexcept { ptr = src; return *this; }

        L_NODISCARD constexpr bool operator==(const pointer& other) const noexcept { return ptr == other.ptr; }
        L_NODISCARD constexpr bool operator!=(const pointer& other) const noexcept { return ptr != other.ptr; }
        L_NODISCARD constexpr bool operator< (const pointer& other) const noexcept { return ptr < other.ptr; }
        L_NODISCARD constexpr bool operator<=(const pointer& other) const noexcept { return ptr <= other.ptr; }
        L_NODISCARD constexpr bool operator> (const pointer& other) const noexcept { return ptr > other.ptr; }
        L_NODISCARD constexpr bool operator>=(const pointer& other) const noexcept { return ptr >= other.ptr; }

        L_NODISCARD constexpr bool operator==(const void* other) const noexcept { return ptr == other; }
        L_NODISCARD constexpr bool operator!=(const void* other) const noexcept { return ptr != other; }
        L_NODISCARD constexpr bool operator< (const void* other) const noexcept { return ptr < other; }
        L_NODISCARD constexpr bool operator<=(const void* other) const noexcept { return ptr <= other; }
        L_NODISCARD constexpr bool operator> (const void* other) const noexcept { return ptr > other; }
        L_NODISCARD constexpr bool operator>=(const void* other) const noexcept { return ptr >= other; }

        L_NODISCARD constexpr bool operator==(std::nullptr_t) const noexcept { return ptr == nullptr; }
        L_NODISCARD constexpr bool operator!=(std::nullptr_t) const noexcept { return ptr != nullptr; }
    };

    template<>
    struct pointer<void>
    {
        void* ptr;

        L_NODISCARD constexpr operator bool() const noexcept { return ptr; }
        L_NODISCARD constexpr operator const void* () const noexcept { return ptr; }
        L_NODISCARD constexpr operator void* () noexcept { return ptr; }
        L_NODISCARD constexpr operator pointer<const void>() noexcept { return { ptr }; }

        constexpr pointer& operator=(void* src) noexcept { ptr = src; return *this; }

        L_NODISCARD constexpr bool operator==(const pointer& other) const noexcept { return ptr == other.ptr; }
        L_NODISCARD constexpr bool operator!=(const pointer& other) const noexcept { return ptr != other.ptr; }
        L_NODISCARD constexpr bool operator< (const pointer& other) const noexcept { return ptr < other.ptr; }
        L_NODISCARD constexpr bool operator<=(const pointer& other) const noexcept { return ptr <= other.ptr; }
        L_NODISCARD constexpr bool operator> (const pointer& other) const noexcept { return ptr > other.ptr; }
        L_NODISCARD constexpr bool operator>=(const pointer& other) const noexcept { return ptr >= other.ptr; }

        L_NODISCARD constexpr bool operator==(const void* other) const noexcept { return ptr == other; }
        L_NODISCARD constexpr bool operator!=(const void* other) const noexcept { return ptr != other; }
        L_NODISCARD constexpr bool operator< (const void* other) const noexcept { return ptr < other; }
        L_NODISCARD constexpr bool operator<=(const void* other) const noexcept { return ptr <= other; }
        L_NODISCARD constexpr bool operator> (const void* other) const noexcept { return ptr > other; }
        L_NODISCARD constexpr bool operator>=(const void* other) const noexcept { return ptr >= other; }

        L_NODISCARD constexpr bool operator==(std::nullptr_t) const noexcept { return ptr == nullptr; }
        L_NODISCARD constexpr bool operator!=(std::nullptr_t) const noexcept { return ptr != nullptr; }
    };
}
