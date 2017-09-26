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

#ifndef REMOTE_MEMORY_READ_MEMORY_INL
#define REMOTE_MEMORY_READ_MEMORY_INL

#include "../read_memory.hpp"
#include <mach/mach_types.h>
#include "../error.hpp"

namespace remote {

    namespace detail {

        extern "C" ::kern_return_t
        mach_vm_read_overwrite(const ::vm_map_t target_task, ::mach_vm_address_t address, ::mach_vm_size_t size
                               , ::mach_vm_address_t data, ::mach_vm_size_t* outsize);

    }

    template<class T, class Address, class Size>
    inline void read_memory(const jm::native_handle_t handle, Address address, T* buffer, Size size)
    {
        ::mach_vm_size_t read;
        const auto       kr = detail::mach_vm_read_overwrite(handle
                                                             , (::mach_vm_address_t)(address)
                                                             , static_cast<::mach_vm_size_t>(size)
                                                             , reinterpret_cast<::mach_vm_address_t>(buffer)
                                                             , &read);
        if (kr != KERN_SUCCESS)
            detail::throw_error(kr, "mach_vm_read_overwrite() failed");
        else if (read != size)
            throw std::range_error("mach_vm_read_overwrite() read less than requested");
    };

    template<class T, class Address, class Size>
    inline void read_memory(const jm::native_handle_t handle, Address address, T* buffer, Size size
                            , std::error_code& ec) noexcept(!jm::detail::checked_pointers)
    {
        ::mach_vm_size_t read;
        const auto       kr = detail::mach_vm_read_overwrite(handle
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
