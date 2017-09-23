#ifndef REMOTE_MEMORY_HPP
#define REMOTE_MEMORY_HPP

#include "remote_memory/utils.hpp"
#include "remote_memory/operations_policy.hpp"


namespace remote {

    template<class OperationsPolicy>
    struct basic_memory : public OperationsPolicy {
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
        template<class T, class Address, class Size>
        void read(Address address, T* buffer, Size size) const
        {
            OperationsPolicy::read(address, buffer, size);
        }
        template<class T, class Address, class Size>
        void read(Address address, T* buffer, Size size, std::error_code& ec) const noexcept
        {
            OperationsPolicy::read(address, buffer, size, ec);
        }

        /// \brief Reads the memory at range [address; address + sizeof(T)] into given buffer.
        /// \param address The address in the target process to read from.
        /// \param buffer The buffer into which the memory will be copied to.
        /// \tparam T The type of buffer to be used.
        template<class T, class Address>
        void read(Address address, T& buffer) const
        {
            OperationsPolicy::read(address, ::std::addressof(buffer), sizeof(T));
        }
        template<class T, class Address>
        void read(Address address, T& buffer, std::error_code& ec) const noexcept
        {
            OperationsPolicy::read(address, ::std::addressof(buffer), sizeof(T), ec);
        }

        /// \brief Reads the memory at range [address; address + sizeof(T)] and returns it as requested type.
        /// \param address The address in the target process to read from.
        /// \tparam T The type of buffer to be used for information to be read to.
        template<class T, class Address>
        T read(Address address) const
        {
            typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type storage;
            OperationsPolicy::read(address, &storage, sizeof(T));
            return *static_cast<T*>(static_cast<void*>(&storage));
        }
        template<class T, class Address>
        T read(Address address, std::error_code& ec) const noexcept
        {
            typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type storage;
            OperationsPolicy::read(address, &storage, sizeof(T), ec);
            return *static_cast<T*>(static_cast<void*>(&storage));
        }

        /// \brief Overwrites the memory at range [address; address + size] with the contents of buffer.
        /// \param address The address in the target process to write to.
        /// \param buffer The buffer of memory to be written.
        /// \param size The size of buffer.
        template<class T, class Address, class Size>
        void write(Address address, const T* buffer, Size size) const
        {
            OperationsPolicy::write(address, buffer, size);
        }
        template<class T, class Address, class Size>
        void write(Address address, const T* buffer, Size size, std::error_code& ec) const noexcept
        {
            OperationsPolicy::write(address, buffer, size, ec);
        }

        /// \brief Overwrites the memory at range [address; address + sizeof(T)] with the contents of buffer.
        /// \param address The address in the target process to write to.
        /// \param buffer The buffer of memory to be written.
        /// \tparam T The type of buffer which will be written into target memory.
        template<class T, class Address>
        void write(Address address, const T& buffer) const
        {
            OperationsPolicy::write(address, std::addressof(buffer), sizeof(T));
        }
        template<class T, class Address>
        void write(Address address, const T& buffer, std::error_code& ec) const noexcept
        {
            OperationsPolicy::write(address, std::addressof(buffer), sizeof(T), ec);
        }

        /// \brief Traverses a pointers chain.
        /// \param base The address of next pointer that will be dereferenced.
        /// \param offset The offset that will be added to the derefenenced pointer.
        /// \param args The offsets that will be recursively added.
        /// \code traverse_pointers_chain(base_address, 0x20, 0x8)
        ///       // same as *(*(*(base_address) + 0x20) + 0x8)
        template <class Ptr, class Base, class Offset, class... Args>
        Ptr traverse_pointers_chain(Base base, Offset offset, Args... args)
        {
            const auto next = read<Ptr>(base) + offset;
            return traverse_pointers_chain(next, args...);
        };
        template <class Ptr, class Base, class Offset>
        Ptr traverse_pointers_chain(Base base, Offset offset)
        {
            return read<Ptr>(base) + offset;
        };
    };

    using memory = basic_memory<operations_policy>;

}

#endif // include guard