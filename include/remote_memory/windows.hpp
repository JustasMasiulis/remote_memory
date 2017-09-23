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

#ifndef REMOTE_MEMORY_WINDOWS_HPP
#define REMOTE_MEMORY_WINDOWS_HPP

#include "process_handle/include/process_handle.hpp"

namespace remote { namespace detail {



    inline std::error_code get_last_error() noexcept
    {
        return std::error_code(static_cast<int>(GetLastError()), std::system_category());
    }

    inline void throw_last_error(const char* msg)
    {
        throw std::system_error(get_last_error(), msg);
    }

    struct memory_traits {
        jm::process_handle _handle;

        using address_type = std::uintptr_t;
        using size_type    = SIZE_T_;

        explicit memory_traits()
                : _handle() {}

        template<typename... Args>
        explicit memory_traits(Args&& ... args)
                : _handle(std::forward<Args>(args)...) {}

    protected:
        template<typename T>
        void read(address_type address, T* buffer, size_type size) const
        {

        }
        template<typename T>
        void read(address_type address, T* buffer, size_type size, std::error_code& ec) const noexcept
        {

        }

        template<typename T>
        void write(address_type address, const T* buffer, size_type size) const
        {
            if (!WriteProcessMemory(my_base::handle(), reinterpret_cast<void*>(address), buffer, size, nullptr))
                throw_last_error("WriteProcessMemory() failed");
        }
        template<typename T>
        void write(address_type address, const T* buffer, size_type size, std::error_code& ec) const noexcept
        {
            if (!WriteProcessMemory(my_base::handle(), reinterpret_cast<void*>(address), buffer, size, nullptr))
                ec = get_last_error();
        }
    };

    using memory_traits       = basic_memory_traits<default_rm_storage>;
    using baseless_memory_traits = basic_memory_traits<baseless_rm_storage>;

}}

#endif // include guard
