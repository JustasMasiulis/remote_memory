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

#include "../error.hpp"
#include "../utils.hpp"
#include <sys/uio.h>

namespace remote {

    /// \brief Overwrites the memory range [address; address + size] with the contents of given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the memory region to which the data will be written into.
    /// \param buffer The buffer whose data will be written into remote memory.
    /// \param size The size of memory region to overwrite.
    /// \throw Throws an std::system_error on failure, std::range_error on partial write
    ///        or std::overflow_error if address is bigger than what the native function supports.
    template<typename T, class Address, class Size>
    inline void write_memory(int handle, Address address, const T* buffer, Size size)
    {
        const ::iovec local  = {buffer, size};
        const ::iovec target = {jm::detail::pointer_cast<void*>(address), size};

        const auto written = ::process_vm_writev(handle, &local, 1, &target, 1, 0);

        if (written == -1)
            detail::throw_last_error("process_vm_writev() failed");
        else if (written < size)
            throw std::range_error("process_vm_writev() wrote less than requested");
    }

    /// \brief Overwrites the memory range [address; address + size] with the contents of given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the memory region to which the data will be written into.
    /// \param buffer The buffer whose data will be written into remote memory.
    /// \param size The size of memory region to overwrite.
    /// \param ec The error code that will be set in case of failure. If the function does not fail
    ///           but performs only a partial write error code is set to result_out_of_range.
    /// \throw May throw an std::overflow_error if the address is out of native address type range.
    template<class T, class Address, class Size>
    inline void write_memory(int handle, Address address, const T* buffer, Size size
                             , std::error_code& ec) noexcept(!jm::detail::checked_pointers)
    {
        const ::iovec local  = {buffer, size};
        const ::iovec target = {jm::detail::pointer_cast<void*>(address), size};

        const auto written = ::process_vm_writev(handle, &local, 1, &target, 1, 0);

        if (written == -1)
            ec = detail::get_last_error();
        else if (written < size)
            ec = std::make_error_code(std::errc::result_out_of_range);
    }

} // namespace remote

#endif // include guard
