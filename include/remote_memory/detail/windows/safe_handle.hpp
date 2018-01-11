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

#ifndef REMOTE_MEMORY_WINDOWS_SAFE_HANDLE_HPP
#define REMOTE_MEMORY_WINDOWS_SAFE_HANDLE_HPP

#include "../../native_types.hpp"
#include "../error.hpp"
#include <memory>

namespace remote { namespace detail {

    extern "C" {
        __declspec(dllimport) int __stdcall CloseHandle(void* handle);
        __declspec(dllimport) void* __stdcall OpenProcess(unsigned long desired_access,
                                                          int           inherit_handle,
                                                          unsigned long process_id);
        __declspec(dllimport) unsigned long __stdcall GetCurrentProcessId();
    }

    constexpr unsigned long SYNCHRONIZE_              = 0x00100000;
    constexpr unsigned long STANDARD_RIGHTS_REQUIRED_ = 0x000F0000;
    constexpr unsigned long PROCESS_ALL_ACCESS_ =
        (STANDARD_RIGHTS_REQUIRED_ | SYNCHRONIZE_ | 0xFFF);

    struct safe_handle {
        std::shared_ptr<void> handle;

        struct handle_deleter_t {
            inline void operator()(native_handle_t handle) noexcept
            {
                if (handle)
                    CloseHandle(handle);
            }
        };

        safe_handle() : safe_handle(GetCurrentProcessId()) {}

        safe_handle(pid_t pid)
            : handle(OpenProcess(PROCESS_ALL_ACCESS_, 0, static_cast<unsigned long>(pid)), handle_deleter_t{})
        {
            if (!handle.get())
                detail::throw_last_error("OpenProcess() failed");
        }

        safe_handle(pid_t pid, std::error_code& ec)
            : handle(OpenProcess(PROCESS_ALL_ACCESS_, 0, static_cast<unsigned long>(pid)), handle_deleter_t{})
        {
            if (!handle.get())
                ec = detail::get_last_error();
        }

        native_handle_t get() const noexcept { return handle.get(); }
    };

}} // namespace remote::detail

#endif // !REMOTE_MEMORY_WINDOWS_SAFE_HANDLE_HPP
