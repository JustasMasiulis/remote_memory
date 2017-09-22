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

namespace remote {

    template<class OperationsPolicy>
    struct basic_memory : public OperationsPolicy {
        /// \brief The type to be used for integral representation of address.
        using address_type = typename OperationsPolicy::address_type;
        /// \brief The type to be used for size.
        using size_type    = typename OperationsPolicy::size_type;

        template<class... Args>
        basic_memory(Args&&... args)
            : OperationsPolicy(std::forward<Args>(args)...) {};

        basic_memory(const basic_memory&) = default;
        basic_memory& operator=(const basic_memory&) = default;

        basic_memory(basic_memory&&) = default;
        basic_memory& operator=(basic_memory&&) = default;

        /// \brief Reads the memory at range [address; address + size] into given buffer.
        /// \param address The address in the target process to read from.
        /// \param buffer The buffer into which the memory at range [address; address + size] will be copied to.
        /// \param size The size of memory region to read.
        template<class T, class Address>
        void read(Address address, T* buffer, size_type size) const
        {
            OperationsPolicy::read(jm::detail::pointer_cast<address_type>(address), buffer, size);
        }
        template<class T, class Address>
        void read(Address address, T* buffer, size_type size, std::error_code& ec) const noexcept
        {
            OperationsPolicy::read(jm::detail::pointer_cast<address_type>(address), buffer, size, ec);
        }

        /// \brief Reads the memory at range [address; address + sizeof(T)] into given buffer.
        /// \param address The address in the target process to read from.
        /// \param buffer The buffer into which the memory will be copied to.
        /// \tparam T The type of buffer to be used.
        template<class T, class Address>
        void read(Address address, T& buffer) const
        {
            OperationsPolicy::read(jm::detail::pointer_cast<address_type>(address), ::std::addressof(buffer), sizeof(T));
        }
        template<class T, class Address>
        void read(Address address, T& buffer, std::error_code& ec) const noexcept
        {
            OperationsPolicy::read(jm::detail::pointer_cast<address_type>(address), ::std::addressof(buffer), sizeof(T), ec);
        }

        /// \brief Reads the memory at range [address; address + sizeof(T)] and returns it as requested type.
        /// \param address The address in the target process to read from.
        /// \tparam T The type of buffer to be used for information to be read to.
        template<class T, class Address>
        T read(Address address) const
        {
            typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type _storage;
            OperationsPolicy::read(jm::detail::pointer_cast<address_type>(address), &_storage, sizeof(T));
            return *static_cast<T*>(static_cast<void*>(&_storage));
        }
        template<class T, class Address>
        T read(Address address, std::error_code& ec) const noexcept
        {
            typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type _storage;
            OperationsPolicy::read(jm::detail::pointer_cast<address_type>(address), &_storage, sizeof(T), ec);
            return *static_cast<T*>(static_cast<void*>(&_storage));
        }

        /// \brief Overwrites the memory at range [address; address + size] with the contents of buffer.
        /// \param address The address in the target process to write to.
        /// \param buffer The buffer of memory to be written.
        /// \param size The size of buffer.
        template<class T, class Address>
        void write(Address address, const T* buffer, std::size_t size) const
        {
            OperationsPolicy::write(jm::detail::pointer_cast<address_type>(address), buffer, size);
        }
        template<class T, class Address>
        void write(Address address, const T* buffer, std::size_t size, std::error_code& ec) const noexcept
        {
            OperationsPolicy::write(jm::detail::pointer_cast<address_type>(address), buffer, size, ec);
        }

        /// \brief Overwrites the memory at range [address; address + sizeof(T)] with the contents of buffer.
        /// \param address The address in the target process to write to.
        /// \param buffer The buffer of memory to be written.
        /// \tparam T The type of buffer which will be written into target memory.
        template<class T, class Address>
        void write(Address address, const T& buffer) const
        {
            OperationsPolicy::write(jm::detail::pointer_cast<address_type>(address), std::addressof(buffer), sizeof(T));
        }
        template<class T, class Address>
        void write(Address address, const T& buffer, std::error_code& ec) const noexcept
        {
            OperationsPolicy::write(jm::detail::pointer_cast<address_type>(address), std::addressof(buffer), sizeof(T), ec);
        }
    };

    using memory = basic_memory<detail::baseless_memory_traits>;

}

#endif // include guard