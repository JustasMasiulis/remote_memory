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

#include "../write_memory.hpp"
#include "definitions.hpp"
#include "../error.hpp"
#include "../utils.hpp"

namespace remote {

    template<typename T, class Address, class Size>
    inline void write_memory(const jm::native_handle_t handle, Address address, const T* buffer, Size size)
    {
        if (!detail::WriteProcessMemory(const_cast<void*>(handle)
                                        , jm::detail::pointer_cast<void*>(address)
                                        , buffer
                                        , size
                                        , nullptr)) {
            const auto code = static_cast<int>(detail::GetLastError());
            if (code == detail::ERROR_PARTIAL_COPY_)
                throw std::range_error("ReadProcessMemory() read less than requested");
            else
                detail::throw_error(code, "ReadProcessMemory() failed");
        }
    }


    template<class T, class Address, class Size>
    inline void write_memory(const jm::native_handle_t handle, Address address, const T* buffer, Size size
                             , std::error_code& ec) noexcept(!jm::detail::checked_pointers)
    {
        if (!detail::WriteProcessMemory(const_cast<void*>(handle)
                                        , jm::detail::pointer_cast<void*>(address)
                                        , buffer
                                        , size
                                        , nullptr)) {
            const auto code = static_cast<int>(jm::detail::GetLastError());
            if (code == detail::ERROR_PARTIAL_COPY_)
                ec = std::make_error_code(std::errc::result_out_of_range);
            else
                ec = std::error_code(code, std::system_category());
        }
    }

} // namespace remote

#endif // include guard
