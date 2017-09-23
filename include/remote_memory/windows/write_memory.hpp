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

#ifndef REMOTE_MEMORY_WRITE_MEMORY_HPP
#define REMOTE_MEMORY_WRITE_MEMORY_HPP

#include "../utils.hpp"
#include "definitions.hpp"

namespace remote {

    /// \brief Overwrites the memory range [address; address + size] with the contents of given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the memory region to which the data will be written into.
    /// \param buffer The buffer whose data will be written into remote memory.
    /// \param size The size of memory region to overwrite.
    /// \throw Throws an std::system_error on failure.
    template<typename T, class Address, class Size>
    inline void write_memory(const void* handle, Address address, const T* buffer, Size size)
    {
        if (!detail::WriteProcessMemory(const_cast<void*>(handle)
                                        , jm::detail::pointer_cast<void*>(address)
                                        , buffer
                                        , size
                                        , nullptr))
            detail::throw_last_error("WriteProcessMemory() failed");
    }

    /// \brief Overwrites the memory range [address; address + size] with the contents of given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the memory region to which the data will be written into.
    /// \param buffer The buffer whose data will be written into remote memory.
    /// \param size The size of memory region to overwrite.
    /// \param ec The error code that will be set in case of failure.
    /// \throw Does not throw.
    template<class T, class Address, class Size>
    inline void write_memory(const void* handle, Address address, const T* buffer, Size size, std::error_code& ec) noexcept
    {
        if (!detail::WriteProcessMemory(const_cast<void*>(handle)
                                        , jm::detail::pointer_cast<void*>(address)
                                        , buffer
                                        , size
                                        , nullptr))
            ec = detail::get_last_error();
    }

} // namespace remote

#endif // include guard
