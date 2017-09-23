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

#ifndef REMOTE_MEMORY_OPERATIONS_POLICY_HPP
#define REMOTE_MEMORY_OPERATIONS_POLICY_HPP

#if defined(_WIN32)
    #include "windows/read_memory.hpp"
    #include "windows/write_memory.hpp"
#elif defined(__linux__)
    #include "linux/read_memory.hpp"
    #include "linux/write_memory.hpp"
#elif defined(__APPLE__)
    #include "osx/read_memory.hpp"
    #include "osx/write_memory.hpp"
#else
    #error "unknown platform"
#endif

namespace remote {

    class operations_policy {
        jm::process_handle _handle;

    public:
        /// \brief Forwards all arguments to the jm::process_handle
        template<class... Args>
        explicit operations_policy(Args&& ... args)
                : _handle(std::forward<Args>(args)...) {}

        template<class T, class Address, class Size>
        inline void read(Address address, T* buffer, Size size) const
        {
            read_memory(_handle.native(), address, buffer, size);
        };

        template<class T, class Address, class Size>
        inline void read(Address address, T* buffer, Size size, std::error_code& ec) const noexcept
        {
            read_memory(_handle.native(), address, buffer, size, ec);
        };

        /// \brief Overwrites the memory range [address; address + size] with the contents of given buffer.
        /// \param address The address of the memory region to which the data will be written into.
        /// \param buffer The buffer whose data will be written into remote memory.
        /// \param size The size of memory region to overwrite.
        /// \throw Throws an std::system_error on failure.
        template<typename T, class Address, class Size>
        inline void write(const void* handle, Address address, const T* buffer, Size size)
        {
            write_memory(_handle.native(), address, buffer, size);
        }

        /// \brief Overwrites the memory range [address; address + size] with the contents of given buffer.
        /// \param address The address of the memory region to which the data will be written into.
        /// \param buffer The buffer whose data will be written into remote memory.
        /// \param size The size of memory region to overwrite.
        /// \param ec The error code that will be set in case of failure.
        /// \throw Does not throw.
        template<class T, class Address, class Size>
        inline void write(Address address, const T* buffer, Size size, std::error_code& ec) noexcept
        {
            write_memory(_handle.native(), address, buffer, size, ec);
        }
    };

} // namespace remote

#endif // include guard
