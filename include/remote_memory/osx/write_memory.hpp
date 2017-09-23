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

#include <mach/mach_vm.c>
#include <system_error>
#include <cstdint>

namespace remote {

    /// \brief Overwrites the memory range [address; address + size] with the contents of given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the memory region to which the data will be written into.
    /// \param buffer The buffer whose data will be written into remote memory.
    /// \param size The size of memory region to overwrite.
    /// \throw Throws an std::system_error on failure.
    template<typename T, class Address, class Size>
    inline void write_memory(::mach_port_t handle, Address address, const T* buffer, Size size)
    {
        const auto kr = ::mach_vm_write(handle
                                        , reinterpret_cast<std::uint64_t>(address)
                                        , reinterpret_cast<std::uintptr_t>(buffer)
                                        , reinterpret_cast<::mach_msg_type_number_t>(size));
        if (kr != KERN_SUCCESS)
            throw std::system_error(std::error_code(kr, std::system_category()), "mach_vm_write() failed");
    }

    /// \brief Overwrites the memory range [address; address + size] with the contents of given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the memory region to which the data will be written into.
    /// \param buffer The buffer whose data will be written into remote memory.
    /// \param size The size of memory region to overwrite.
    /// \param ec The error code that will be set in case of failure.
    /// \throw Does not throw.
    template<class T, class Address, class Size>
    inline void
    write_memory(::mach_port_t handle, Address address, const T* buffer, Size size, std::error_code& ec) noexcept
    {
        const auto kr = ::mach_vm_write(handle
                                        , reinterpret_cast<std::uint64_t>(address)
                                        , reinterpret_cast<std::uintptr_t>(buffer)
                                        , reinterpret_cast<::mach_msg_type_number_t>(size));
        if (kr != KERN_SUCCESS)
            ec = std::error_code(kr, std::system_category());
    }

} // namespace remote

#endif // include guard
