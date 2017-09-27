/*
* Copyright 2017 Justas Masiulis
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef REMOTE_MEMORY_HPP
#define REMOTE_MEMORY_HPP

#include "remote_memory/operations_policy.hpp"

#ifndef REMOTE_MEMORY_UNSAFE_READS
    #include <memory>
#endif

#ifndef REMOTE_MEMORY_DISABLE_TRIVIAL_COPY_CHECKS
    #define REMOTE_MEMORY_TRIVIAL_COPY_CHECK static_assert(std::is_trivially_copyable<T>::value, \
                                                           "the type must be trivially copyable for a safe read or write to occur" \
                                                           "to disable this warning define REMOTE_MEMORY_DISABLE_TRIVIAL_COPY_CHECKS");
#else
    #define REMOTE_MEMORY_TRIVIAL_COPY_CHECK
#endif

namespace remote {

    template<class OperationsPolicy>
    struct basic_memory : public OperationsPolicy {
        template<class... Args>
        explicit basic_memory(Args&& ... args) noexcept(std::is_nothrow_constructible<OperationsPolicy, Args...>::value)
                : OperationsPolicy(std::forward<Args>(args)...) {};

        /// \brief Refer to remote::read_memory.
        ///        The parameters have the same meaning, but does not require a process handle.
        /// \throw Strong exception safety guarantee - if the function throws the buffer state is left unchanged.
        template<class T, class Address, class Size>
        void read(Address address, T* buffer, Size size) const
        {
            REMOTE_MEMORY_TRIVIAL_COPY_CHECK
            // if the read fails we don't want to leave the object in an invalid state
#ifndef REMOTE_MEMORY_UNSAFE_READS
            auto temp = std::make_unique<std::uint8_t[]>(size);
            OperationsPolicy::read(address, temp.get(), sizeof(T));
            std::memcpy(buffer, temp.get(), sizeof(T));
#else
            OperationsPolicy::read(address, temp.get(), sizeof(T));
#endif
        }
        template<class T, class Address, class Size>
        void read(Address address, T* buffer, Size size, std::error_code& ec) const
        {
            REMOTE_MEMORY_TRIVIAL_COPY_CHECK
#ifndef REMOTE_MEMORY_UNSAFE_READS
            auto temp = std::make_unique<std::uint8_t[]>(size);
            OperationsPolicy::read(address, temp.get(), sizeof(T), ec);
            if (!ec)
                std::memcpy(buffer, temp.get(), sizeof(T));
#else
            OperationsPolicy::read(address, temp.get(), sizeof(T), ec);
#endif
        }

        /// \brief Refer to remote::read_memory.
        ///        The parameters have the same meaning, but does not require a process handle.
        ///        The size will be sizeof(T).
        /// \throw Strong exception safety guarantee - if the function throws the buffer state is left unchanged.
        template<class T, class Address>
        void read(Address address, T& buffer) const
        {
            REMOTE_MEMORY_TRIVIAL_COPY_CHECK
#ifndef REMOTE_MEMORY_UNSAFE_READS
            std::uint8_t temp[sizeof(T)];
            OperationsPolicy::read(address, &temp, sizeof(T));
            std::memcpy(std::addressof(buffer), temp, sizeof(T));
#else
            OperationsPolicy::read(address, &temp, sizeof(T));
#endif
        }
        template<class T, class Address>
        void read(Address address, T& buffer, std::error_code& ec) const noexcept
        {
            REMOTE_MEMORY_TRIVIAL_COPY_CHECK
#ifndef REMOTE_MEMORY_UNSAFE_READS
            std::uint8_t temp[sizeof(T)];
            OperationsPolicy::read(address, &temp, sizeof(T), ec);
            if (!ec)
                std::memcpy(std::addressof(buffer), temp, sizeof(T));
#else
            OperationsPolicy::read(address, &temp, sizeof(T), ec);
#endif
        }

        /// \brief Refer to remote::read_memory which is by default used internally.
        ///        The memory will be copied into a buffer of type T.
        template<class T, class Address>
        T read(Address address) const
        {
            REMOTE_MEMORY_TRIVIAL_COPY_CHECK
            typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type storage;
            OperationsPolicy::read(address, &storage, sizeof(T));
            return *static_cast<T*>(static_cast<void*>(&storage));
        }
        /// \brief Refer to remote::read_memory which is by default used internally.
        ///        The memory will be copied into a buffer of type T. If the function fails
        ///        you will be returned a default constructed.
        template<class T, class Address>
        T read(Address address, std::error_code& ec) const noexcept
        {
            REMOTE_MEMORY_TRIVIAL_COPY_CHECK
            typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type storage;
            OperationsPolicy::read(address, &storage, sizeof(T), ec);
            if (ec)
                return T{};

            return *static_cast<T*>(static_cast<void*>(&storage));
        }

        /// \brief refer to remote::write_memory.
        template<class T, class Address, class Size>
        void write(Address address, const T* buffer, Size size) const
        {
            REMOTE_MEMORY_TRIVIAL_COPY_CHECK
            OperationsPolicy::write(address, buffer, size);
        }
        template<class T, class Address, class Size>
        void write(Address address, const T* buffer, Size size, std::error_code& ec) const noexcept
        {
            REMOTE_MEMORY_TRIVIAL_COPY_CHECK
            OperationsPolicy::write(address, buffer, size, ec);
        }

        /// \brief refer to remote::write_memory. Size will be sizeof(T).
        template<class T, class Address>
        void write(Address address, const T& buffer) const
        {
            REMOTE_MEMORY_TRIVIAL_COPY_CHECK
            OperationsPolicy::write(address, std::addressof(buffer), sizeof(T));
        }
        template<class T, class Address>
        void write(Address address, const T& buffer, std::error_code& ec) const noexcept
        {
            REMOTE_MEMORY_TRIVIAL_COPY_CHECK
            OperationsPolicy::write(address, std::addressof(buffer), sizeof(T), ec);
        }

        /// \brief Traverses a pointers chain.
        /// \param base The address of next pointer that will be dereferenced.
        /// \param offset The offset that will be added to the derefenenced pointer.
        /// \param args The offsets that will be recursively added.
        /// \code traverse_pointers_chain(base_address, 0x20, 0x8)
        ///       // same as *(*(*(base_address) + 0x20) + 0x8)
        /// \throw Throws an exception on failure. Refer to remote::read_memory.
        template<class Ptr = std::uintptr_t, class Base, class Offset, class... Args>
        Ptr traverse_pointers_chain(Base base, Offset offset, Args... args)
        {
            jm::detail::as_uintptr_t<sizeof(Ptr)> next;
            read(base, next);
            return traverse_pointers_chain(next + offset, args...);
        };
        template<class Ptr = std::uintptr_t, class Base, class Offset>
        Ptr traverse_pointers_chain(Base base, Offset offset)
        {
            jm::detail::as_uintptr_t<sizeof(Ptr)> next;
            read(base, next);
            return read<Ptr>(next + offset);
        };
    };

    using memory = basic_memory<operations_policy>;

}

#endif // include guard