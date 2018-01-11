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

#ifndef REMOTE_MEMORY_WRITE_MEMORY_INL
#define REMOTE_MEMORY_WRITE_MEMORY_INL

#include "../../write_memory.hpp"
#include "../error.hpp"
#include <sys/uio.h>

namespace remote {

    template<typename T, class Address, class Size>
    inline void write_memory(const native_handle_t handle, Address address, const T* buffer, Size size)
    {
        const ::iovec local  = {const_cast<T*>(buffer), size};
        const ::iovec target = {jm::detail::pointer_cast<void*>(address), size};

        const auto written = ::process_vm_writev(handle, &local, 1, &target, 1, 0);

        if (written == -1)
            detail::throw_last_error("process_vm_writev() failed");
        else if (written < size)
            throw std::range_error("process_vm_writev() wrote less than requested");
    }


    template<class T, class Address, class Size>
    inline void write_memory(const native_handle_t handle, Address address, const T* buffer, Size size
                             , std::error_code& ec) noexcept(!jm::detail::checked_pointers)
    {
        const ::iovec local  = {const_cast<T*>(buffer), size};
        const ::iovec target = {jm::detail::pointer_cast<void*>(address), size};

        const auto written = ::process_vm_writev(handle, &local, 1, &target, 1, 0);

        if (written == -1)
            ec = detail::get_last_error();
        else if (written < size)
            ec = std::make_error_code(std::errc::result_out_of_range);
    }

} // namespace remote

#endif // include guard
