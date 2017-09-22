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

#ifndef JM_REMOTE_MEMORY_WINDOWS_HPP
#define JM_REMOTE_MEMORY_WINDOWS_HPP

#include <cstdint>

namespace jm { namespace detail {

#ifdef _WIN64
    #if defined(__CYGWIN__)
    typedef unsigned long SIZE_T_;
    #else
    typedef unsigned __int64 SIZE_T_;
    #endif
#else
    typedef unsigned long SIZE_T_;
#endif

    extern "C" {
    __declspec(dllimport) int __stdcall
    ReadProcessMemory(void* process_handle, const void* base_address, void* buffer, SIZE_T_ size, SIZE_T_* bytes_read);

    __declspec(dllimport) int __stdcall
    WriteProcessMemory(void* process_handle, void* base_address, const void* buffer, SIZE_T_ size, SIZE_T_* bytes_read);
    }

    inline std::error_code get_last_error() noexcept
    {
        return std::error_code(static_cast<int>(GetLastError()), std::system_category());
    }

    inline void throw_last_error(const char* msg)
    {
        throw std::system_error(get_last_error(), msg);
    }

    template<class TraitsType>
    class default_rm_storage {
        typename TraitsType::address_type _base_address;
        void*                             _handle;

    public:
        const typename TraitsType::address_type& base_address() const noexcept { return _base_address; }

        typename TraitsType::address_type& base_address() noexcept { return _base_address; }

        void* handle() const noexcept { return _handle.native(); }
    };

    template<class TraitsType>
    class baseless_rm_storage {
        void* _handle;

    protected:
        typename TraitsType::address_type base_address() const noexcept { return 0; }

    public:
        void* handle() const noexcept { return _handle; }
    };

    template<template<class> class Storage>
    struct basic_memory_traits : public Storage<basic_memory_traits> {
        using address_type = std::uintptr_t;
        using size_type    = SIZE_T_;

        template<typename... Args>
        basic_memory_traits(Args&&... args)
                : Storage(std::forward<Args>(args)...) {}

        // TODO add all operators

    protected:
        ~basic_memory_traits() noexcept = default;

        template<typename T>
        void read(address_type address, T* buffer, size_type size) const
        {
            if (!ReadProcessMemory(Storage::handle()
                                   , reinterpret_cast<const void*>(address + Storage::base_address())
                                   , buffer
                                   , size
                                   , nullptr))
                throw_last_error("ReadProcessMemory() failed");
        }
        template<typename T>
        void read(address_type address, T* buffer, size_type size, std::error_code& ec) const noexcept
        {
            if (!ReadProcessMemory(Storage::handle()
                                   , reinterpret_cast<const void*>(address + Storage::base_address())
                                   , buffer
                                   , size
                                   , nullptr))
                ec = get_last_error();
        }

        template<typename T>
        void write(address_type address, const T* buffer, size_type size) const
        {
            if (!WriteProcessMemory(Storage::handle()
                                    , reinterpret_cast<void*>(address + Storage::base_address())
                                    , buffer
                                    , size
                                    , nullptr))
                throw_last_error("WriteProcessMemory() failed");
        }

        template<typename T>
        void write(address_type address, const T* buffer, size_type size, std::error_code& ec) const noexcept
        {
            if (!WriteProcessMemory(Storage::handle()
                                    , reinterpret_cast<void*>(address + Storage::base_address())
                                    , buffer
                                    , size
                                    , nullptr))
                ec = get_last_error();
        }
    };

    using memory_traits       = basic_memory_traits<default_rm_storage>;
    using small_memory_traits = basic_memory_traits<baseless_rm_storage>;

}}

#endif // include guard
