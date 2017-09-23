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

#ifndef REMOTE_MEMORY_READ_MEMORY_HPP
#define REMOTE_MEMORY_READ_MEMORY_HPP

#include "../utils.hpp"

namespace remote {

    namespace detail {

        extern "C" __declspec(dllimport) int __stdcall
        ReadProcessMemory(void* process_handle, const void* base_address, void* buffer, SIZE_T_ size
                          , SIZE_T_* bytes_read);

    } // namespace remote::detail

    /// \brief Reads remote memory range [address; address + size] into given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the memory region that will be read.
    /// \param buffer The buffer into which the memory will be read into.
    /// \param size The size of memory region to read into memory buffer.
    /// \throw Throws an std::system_error on failure.
    template<class T, class Address, class Size>
    inline void read_memory(const void* handle, Address address, T* buffer, Size size)
    {
        // the handle won't get modified so we can take it as const and then cast it away
        if (!detail::ReadProcessMemory(const_cast<void*>(handle)
                                       , detail::pointer_cast<const void*>(address)
                                       , buffer
                                       , size
                                       , nullptr))
            throw_last_error("ReadProcessMemory() failed");
    };

    /// \brief Reads remote memory range [address; address + size] into given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the memory region that will be read.
    /// \param buffer The buffer into which the memory will be read into.
    /// \param size The size of memory region to read into memory buffer.
    /// \param ec The error code that will be set on failure.
    /// \throw Does not throw.
    template<class T, class Address, class Size>
    inline void read_memory(const void* handle, Address address, T* buffer, Size size, std::error_code& ec) noexcept
    {
        if (!detail::ReadProcessMemory(const_cast<void*>(handle)
                                       , detail::pointer_cast<const void*>(address)
                                       , buffer
                                       , size
                                       , nullptr))
            ec = get_last_error();
    };

} // namespace remote

#endif // include guard
