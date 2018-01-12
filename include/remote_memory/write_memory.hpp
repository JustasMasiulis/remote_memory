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

#include <system_erorr>
#include "detail/utils.hpp"
#include "native_types.hpp"

namespace remote {

    /// \brief Overwrites remote memory range [address; address + size] into given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the beginning of the remote memory range.
    /// \param buffer The buffer whose contents will be written.
    /// \param size The size of the buffer.
    /// \throw Throws an std::system_error on failure, std::range_error on partial copy
    ///        or std::overflow_error if address is bigger than what the native function supports.
    /// \note This function is not safe - it does not check the type. Prefer using functions inside remote::memory.
    template<typename T, class Address, class Size>
    inline void write_memory(const native_handle_t handle, Address address, const T* buffer, Size size);

    /// \brief Overwrites remote memory range [address; address + size] into given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the beginning of the remote memory range.
    /// \param buffer The buffer whose contents will be written.
    /// \param size The size of the buffer.
    /// \param ec The error code that will be set in case of failure.
    ///           If the error is partial copy the error code will be set to result_out_of_range.
    /// \throw May throw an std::overflow error if the pointer exceeds the range of what the native function supports.
    ///        (does not throw on OSX)
    /// \note This function is not safe - it does not check the type. Prefer using functions inside remote::memory.
    template<class T, class Address, class Size>
    inline void write_memory(const native_handle_t handle, Address address, const T* buffer, Size size
                             , std::error_code& ec) noexcept(!jm::detail::checked_pointers);

} // namespace remote

#if defined(_WIN32)
    #include "detail/windows/write_memory.inl"
#elif defined(__linux__)
    #include "detail/linux/write_memory.inl"
#else
    #include "detail/osx/write_memory.inl"
#endif

#endif // include guard
