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

#include "process_handle/include/process_handle.hpp"

namespace remote {

    /// \brief Overwrites the memory range [address; address + size] with the contents of given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the memory region to which the data will be written into.
    /// \param buffer The buffer whose data will be written into remote memory.
    /// \param size The size of memory region to overwrite.
    /// \throw Throws an std::system_error on failure, std::range_error on partial write
    ///        or std::overflow_error if address is bigger than what the native function supports.
    template<typename T, class Address, class Size>
    inline void write_memory(const jm::native_handle_t handle, Address address, const T* buffer, Size size);

    template<class T, class Address, class Size>
    inline void write_memory(const jm::native_handle_t handle, Address address, const T* buffer, Size size
                             , std::error_code& ec) noexcept(!jm::detail::checked_pointers);

} // namespace remote

#if defined(_WIN32)
    #include "windows/write_memory.inl"
#elif defined(__linux__)
    #include "linux/write_memory.inl"
#else
    #include "osx/write_memory.inl"
#endif

#endif // include guard
