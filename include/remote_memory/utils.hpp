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

#ifndef JM_REMOTE_MEMORY_UTILS_HPP
#define JM_REMOTE_MEMORY_UTILS_HPP

#include <stdexcept>
#include <limits>
#include "process_handle/include/process_handle.hpp"

namespace jm { namespace detail {

    template<std::size_t S>
    struct uintptr_adaptive;

    template<>
    struct uintptr_adaptive<4> {
        using type = std::uint32_t;
    };
    template<>
    struct uintptr_adaptive<8> {
        using type = std::uint64_t;
    };

    template<typename Px, typename Py>
    constexpr Px pointer_cast(Py ptr)
    {
#if !defined(REMOTE_MEMORY_NO_PTR_CHECKING)
        if (ptr > std::numeric_limits<typename uintptr_adaptive<sizeof(Px)>::type>::max())
            throw std::overflow_error("attempt to cast to pointer of insufficient size");
#endif

        return (Px) (ptr);
    }

    template<typename Px>
    constexpr Px pointer_cast(Px ptr) noexcept
    {
        ptr;
    }

}}

#endif // include guard
