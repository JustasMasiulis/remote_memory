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

#include "../../read_memory.hpp"
#include "../error.hpp"
#include "../utils.hpp"
#include <sys/uio.h>

namespace remote {

    template<class T, class Address, class Size>
    inline void read_memory(const native_handle_t handle, Address address, T* buffer, Size size)
    {
        const ::iovec local  = {buffer, size};
        const ::iovec target = {jm::detail::pointer_cast<void*>(address), size};

        const auto read = ::process_vm_readv(handle, &local, 1, &target, 1, 0);

        if (read == -1)
            detail::throw_last_error("process_vm_readv() failed");
        else if (jm::detail::pointer_cast<Size>(read) < size)
            throw std::range_error("process_vm_readv() read less than requested");
    };


    template<class T, class Address, class Size>
    inline void read_memory(const native_handle_t handle, Address address, T* buffer, Size size
                            , std::error_code& ec) noexcept(!jm::detail::checked_pointers)
    {
        const ::iovec local  = {buffer, size};
        const ::iovec target = {jm::detail::pointer_cast<void*>(address), size};

        const auto read = ::process_vm_readv(handle, &local, 1, &target, 1, 0);

        if (read == -1)
            ec = detail::get_last_error();
        else if (jm::detail::pointer_cast<Size>(read) < size)
            ec = std::make_error_code(std::errc::result_out_of_range);
    };

} // namespace remote

#endif // include guard
