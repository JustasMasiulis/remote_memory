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

#include "process_handle/include/process_handle.hpp"
#include "utils.hpp"

namespace remote {

    /// \brief Reads remote memory range [address; address + size] into given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the memory region that will be read.
    /// \param buffer The buffer into which the memory will be read into.
    /// \param size The size of memory region to read into memory buffer.
    /// \throw Throws an std::system_error on failure, std::range_error on partial read
    ///        or std::overflow_error if address is bigger than what the native function supports.
    template<class T, class Address, class Size>
    void read_memory(const jm::native_handle_t handle, Address address, T* buffer, Size size);

    /// \brief Reads remote memory range [address; address + size] into given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the memory region that will be read.
    /// \param buffer The buffer into which the memory will be read into.
    /// \param size The size of memory region to read into memory buffer.
    /// \param ec The error code that will be set in case of failure. If the function does not fail
    ///           but performs only a partial read error code is set to result_out_of_range.
    /// \throw May throw an std::overflow_error if the address is out of native address type range.
    template<class T, class Address, class Size>
    void read_memory(const jm::native_handle_t handle, Address address, T* buffer, Size size
                     , std::error_code& ec) noexcept(!jm::detail::checked_pointers);

} // namespace remote

#if defined(_WIN32)
    #include "windows/read_memory.inl"
#elif defined(__linux__)
    #include "linux/read_memory.inl"
#else
    #include "osx/read_memory.inl"
#endif

#endif // include guard
