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

namespace remote {

    template<class OperationsPolicy>
    struct basic_memory : public OperationsPolicy {
        template<class... Args>
        explicit basic_memory(Args&& ... args) noexcept(std::is_nothrow_constructible<OperationsPolicy, Args...>::value)
                : OperationsPolicy(std::forward<Args>(args)...) {};

        basic_memory(const basic_memory&) noexcept(std::is_nothrow_copy_constructible<OperationsPolicy>::value)= default;
        basic_memory& operator=(const basic_memory&) noexcept(std::is_nothrow_copy_assignable<OperationsPolicy>::value) = default;

        basic_memory(basic_memory&&) noexcept(std::is_nothrow_move_constructible<OperationsPolicy>::value) = default;
        basic_memory& operator=(basic_memory&&) noexcept(std::is_nothrow_move_assignable<OperationsPolicy>::value) = default;

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