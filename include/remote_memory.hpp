#ifndef REMOTE_MEMORY_HPP
#define REMOTE_MEMORY_HPP

#include "remote_memory/utils.hpp"
#if defined(_WIN32)
    #include "remote_memory/windows.hpp"
#elif defined(__linux__)
#elif defined(__APPLE__)
#else
    #error "unknown platform"
#endif

namespace jm {

    template<class MemoryTraits>
    struct basic_remote_memory : public MemoryTraits {
        using address_type = MemoryTraits::address_type;
        using size_type    = MemoryTraits::size_type;

        template <class T, class Address>
        void read(Address address, T* buffer, size_type size) const
        {
            MemoryTraits::read(detail::pointer_cast<address_type>(address), buffer, size);
        }

        template <class T, class Address>
        void read(Address address, T& buffer) const
        {
            MemoryTraits::read(detail::pointer_cast<address_type>(address), ::std::addressof(buffer), sizeof(T));
        }

        template<class T, class Address>
        T read(Address address) const
        {
            typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type _storage;
            MemoryTraits::read(detail::pointer_cast<address_type>(address), &_storage, sizeof(T));
            return *static_cast<T*>(static_cast<void*>(&_storage));
        }

        template<class T, class Address>
        void write(Address address, const T* buffer, std::size_t size) const
        {
            MemoryTraits::write(detail::pointer_cast<address_type>(address), buffer, size);
        }

        template<class T, class Address>
        void write(Address address, const T& buffer) const
        {
            MemoryTraits::write(detail::pointer_cast<address_type>(address), std::addressof(buffer), sizeof(T));
        }

    };

    using remote_memory = basic_remote_memory<detail::memory_traits>;

}

#endif // include guard