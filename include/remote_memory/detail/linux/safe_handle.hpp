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

#ifndef REMOTE_MEMORY_LINUX_SAFE_HANDLE_HPP
#define REMOTE_MEMORY_LINUX_SAFE_HANDLE_HPP

#include "../../native_types.hpp"
#include <unistd.h>
#include <system_error>

namespace remote { namespace detail {

    struct safe_handle {
        pid_t process_id;

        safe_handle() noexcept : process_id(::getpid()) {}

        safe_handle(pid_t pid, std::error_code&) noexcept : process_id(pid) {}

        constexpr pid_t get() const noexcept { return process_id; }
    };

}} // namespace remote::detail

#endif // !REMOTE_MEMORY_LINUX_SAFE_HANDLE_HPP
