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

#include <system_error>

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

    template<class AddressType, class SizeType>
    class default_rm_storage {
        AddressType _base_address;
        void*       _handle;

    public:
        const AddressType& base_address() const noexcept { return _base_address; }

        AddressType& base_address() noexcept { return _base_address; }

    protected:
        void* handle() const noexcept { return _handle; }
    };

    template<class AddressType, class SizeType>
    class baseless_rm_storage {
        void* _handle;

    protected:
        AddressType base_address() const noexcept { return 0; }

        void* handle() const noexcept { return _handle; }
    };

    template<template<class, class> class Storage>
    class basic_memory_traits : public Storage<std::uintptr_t, SIZE_T_> {
        using my_base = Storage<std::uintptr_t, SIZE_T_>;

    public:
        using address_type = std::uintptr_t;
        using size_type    = SIZE_T_;

        template<typename... Args>
        basic_memory_traits(Args&& ... args)
                : my_base(std::forward<Args>(args)...) {}


    protected:
        ~basic_memory_traits() noexcept = default;

        template<typename T>
        void read(address_type address, T* buffer, size_type size) const
        {
            if (!ReadProcessMemory(my_base::handle()
                                   , reinterpret_cast<const void*>(address + my_base::base_address())
                                   , buffer
                                   , size
                                   , nullptr))
                throw_last_error("ReadProcessMemory() failed");
        }
        template<typename T>
        void read(address_type address, T* buffer, size_type size, std::error_code& ec) const noexcept
        {
            if (!ReadProcessMemory(my_base::handle()
                                   , reinterpret_cast<const void*>(address + my_base::base_address())
                                   , buffer
                                   , size
                                   , nullptr))
                ec = get_last_error();
        }

        template<typename T>
        void write(address_type address, const T* buffer, size_type size) const
        {
            if (!WriteProcessMemory(my_base::handle()
                                    , reinterpret_cast<void*>(address + my_base::base_address())
                                    , buffer
                                    , size
                                    , nullptr))
                throw_last_error("WriteProcessMemory() failed");
        }
        template<typename T>
        void write(address_type address, const T* buffer, size_type size, std::error_code& ec) const noexcept
        {
            if (!WriteProcessMemory(my_base::handle()
                                    , reinterpret_cast<void*>(address + my_base::base_address())
                                    , buffer
                                    , size
                                    , nullptr))
                ec = get_last_error();
        }
    };

    using memory_traits       = basic_memory_traits<default_rm_storage>;
    using baseless_memory_traits = basic_memory_traits<baseless_rm_storage>;

}}

#endif // include guard
