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

namespace jm { namespace detail {

    #if defined(REMOTE_MEMORY_NO_PTR_CHECKING)
    constexpr static bool checked_pointers = false;
    #else
    constexpr static bool checked_pointers = true;
    #endif

    template<std::size_t S>
    struct as_uintptr;

    template<>
    struct as_uintptr<4> {
        using type = std::uint32_t;
    };
    template<>
    struct as_uintptr<8> {
        using type = std::uint64_t;
    };

    template<std::size_t Size> using as_uintptr_t = typename as_uintptr<Size>::type;

    template<bool>
    struct pointer_checker {
        template<class P2, class P1>
        inline static constexpr void check(P1) noexcept {}
    };

#if !defined(REMOTE_MEMORY_NO_PTR_CHECKING)

    template<>
    struct pointer_checker<true> {
        template<class P2, class P1>
        inline static constexpr void check(P1 p1)
        {
            if (reinterpret_cast<as_uintptr_t<sizeof(P1)>>(p1) > std::numeric_limits<as_uintptr_t<sizeof(P2)>>::max())
                throw std::overflow_error("attempt to cast to pointer of insufficient size");
        }
    };

#endif

    template<typename Px, typename Py>
    inline constexpr Px pointer_cast(Py ptr) noexcept(!checked_pointers)
    {
        using my_pointer_checker = pointer_checker<(sizeof(Py) > sizeof(Px))>;
        my_pointer_checker::template check<Px>(ptr);

        return (Px) (ptr);
    }

}}

#endif // include guard
