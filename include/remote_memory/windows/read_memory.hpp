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

#include "../error.hpp"
#include "../utils.hpp"
#include "definitions.hpp"

namespace remote {

    /// \brief Reads remote memory range [address; address + size] into given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the memory region that will be read.
    /// \param buffer The buffer into which the memory will be read into.
    /// \param size The size of memory region to read into memory buffer.
    /// \throw Throws an std::system_error on failure or std::overflow_error
    ///        if address is out of native address type range.
    template<class T, class Address, class Size>
    inline void read_memory(const void* handle, Address address, T* buffer, Size size)
    {
        // the handle won't get modified so we can take it as const and then cast it away
        if (!detail::ReadProcessMemory(const_cast<void*>(handle)
                                       , jm::detail::pointer_cast<const void*>(address)
                                       , buffer
                                       , size
                                       , nullptr)) {
            const auto error = static_cast<int>(jm::detail::GetLastError());
            if (error == detail::ERROR_PARTIAL_COPY_)
                throw std::range_error("ReadProcessMemory() read less than requested");
            else
                detail::throw_error(error, "ReadProcessMemory() failed");
        }
    };

    /// \brief Reads remote memory range [address; address + size] into given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the memory region that will be read.
    /// \param buffer The buffer into which the memory will be read into.
    /// \param size The size of memory region to read into memory buffer.
    /// \param ec The error code that will be set on failure.
    /// \throw May throw an std::overflow_error if the address is out of native address type range.
    template<class T, class Address, class Size>
    inline void read_memory(const void* handle, Address address, T* buffer, Size size
                            , std::error_code& ec) noexcept(!jm::detail::checked_pointers)
    {
        if (!detail::ReadProcessMemory(const_cast<void*>(handle)
                                       , jm::detail::pointer_cast<const void*>(address)
                                       , buffer
                                       , size
                                       , nullptr)) {
            auto code = detail::get_last_error();
            if (code.value() == detail::ERROR_PARTIAL_COPY_)
                ec = std::make_error_code(std::errc::result_out_of_range);
            else
                ec = std::move(code);
        }
    };

} // namespace remote

#endif // include guard
