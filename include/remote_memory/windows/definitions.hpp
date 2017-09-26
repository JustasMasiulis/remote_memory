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

#ifndef REMOTE_MEMORY_DEFINITIONS_HPP
#define REMOTE_MEMORY_DEFINITIONS_HPP

namespace remote { namespace detail {

#ifdef _WIN64
    #if defined(__CYGWIN__)
        typedef unsigned long SIZE_T_;
    #else
        typedef unsigned __int64 SIZE_T_;
    #endif
#else
    typedef unsigned long SIZE_T_;
#endif

    extern "C" __declspec(dllimport) int __stdcall
    ReadProcessMemory(void* process_handle, const void* base_address, void* buffer, SIZE_T_ size, SIZE_T_* bytes_read);

    extern "C" __declspec(dllimport) int __stdcall
    WriteProcessMemory(void* process_handle, void* base_address, const void* buffer, SIZE_T_ size, SIZE_T_* bytes_read);

    static constexpr int ERROR_PARTIAL_COPY_ = 299;

}} // namespace remote::detail

#endif // include guard
