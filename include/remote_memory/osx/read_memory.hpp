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

#include <mach/mach_types.h>
#include <system_error>
#include <cstdint>

namespace remote {

    namespace detail {

        extern "C" ::kern_return_t
        mach_vm_read_overwrite(::vm_map_t target_task, ::mach_vm_address_t address, ::mach_vm_size_t size
                               , ::mach_vm_address_t data, ::mach_vm_size_t* outsize);

    }

    /// \brief Reads remote memory range [address; address + size] into given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the memory region that will be read.
    /// \param buffer The buffer into which the memory will be read into.
    /// \param size The size of memory region to read into memory buffer.
    /// \throw Throws an std::system_error on failure.
    template<class T, class Address, class Size>
    inline void read_memory(::mach_port_t handle, Address address, T* buffer, Size size)
    {
        std::uint64_t read;
        const auto    kr = detail::mach_vm_read_overwrite(handle
                                                          , (::mach_vm_address_t)(address)
                                                          , static_cast<::mach_vm_size_t>(size)
                                                          , reinterpret_cast<::mach_vm_address_t>(buffer)
                                                          , &read);
        if (kr != KERN_SUCCESS)
            throw std::system_error(std::error_code(kr, std::system_category()), "mach_vm_read_overwrite() failed");
        else if(read != size)
            throw std::range_error("mach_vm_read_overwrite() read less than requested");
    };

    /// \brief Reads remote memory range [address; address + size] into given buffer.
    /// \param handle The handle to remote process.
    /// \param address The address of the memory region that will be read.
    /// \param buffer The buffer into which the memory will be read into.
    /// \param size The size of memory region to read into memory buffer.
    /// \param ec The error code that will be set on failure.
    /// \throw Does not throw.
    template<class T, class Address, class Size>
    inline void read_memory(::mach_port_t handle, Address address, T* buffer, Size size, std::error_code& ec) noexcept
    {
        std::uint64_t read;
        const auto    kr = detail::mach_vm_read_overwrite(handle
                                                          , (::mach_vm_address_t)(address)
                                                          , static_cast<::mach_vm_size_t>(size)
                                                          , reinterpret_cast<::mach_vm_address_t>(buffer)
                                                          , &read);
        if (kr != KERN_SUCCESS)
            ec = std::error_code(kr, std::system_category());
        else if (read != size)
            ec = std::make_error_code(std::errc::result_out_of_range);
    };

} // namespace remote

#endif // include guard
