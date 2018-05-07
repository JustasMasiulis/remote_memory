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

#ifndef REMOTE_MEMORY_OSX_SAFE_HANDLE_HPP
#define REMOTE_MEMORY_OSX_SAFE_HANDLE_HPP

#include "../../native_types.hpp"
#include "../error.hpp"
#include <mach/mach.h>

namespace remote { namespace detail {

    struct safe_handle {
        native_handle_t task;

        safe_handle() noexcept : task(::mach_task_self()) {}

        safe_handle(pid_t pid)
        {
            const auto kr = ::task_for_pid(::mach_task_self(), pid, &task);
            if (kr != KERN_SUCCESS)
                throw std::system_error(std::error_code(kr, std::system_category()),
                                        "task_for_pid() failed");
        }

        safe_handle(pid_t pid, std::error_code& ec) noexcept
        {
            const auto kr = ::task_for_pid(::mach_task_self(), pid, &task);
            if (kr != KERN_SUCCESS)
                ec = std::error_code(kr, std::system_category());
        }

        constexpr native_handle_t get() const noexcept { return task; }
    };

}} // namespace remote::detail

#endif // !REMOTE_MEMORY_OSX_SAFE_HANDLE_HPP
