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

#ifndef REMOTE_MEMORY_LINUX_HPP_HPP
#define REMOTE_MEMORY_LINUX_HPP_HPP

namespace remote { namespace detail {

    template<class AddressType, class SizeType>
    class default_rm_storage {
        AddressType _base_address;
        int         _handle;

    public:
        const AddressType& base_address() const noexcept { return _base_address; }

        AddressType& base_address() noexcept { return _base_address; }

    protected:
        int handle() const noexcept { return _handle; }
    };

    template<class AddressType, class SizeType>
    class baseless_rm_storage {
        int _handle;

    protected:
        AddressType base_address() const noexcept { return 0; }

        int handle() const noexcept { return _handle; }
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
            const ::iovec local  = { buffer, size };
            const ::iovec target = { reinterpret_cast<void*>(address + my_base::base_address()), size };

            const auto read = ::process_vm_readv(my_base::handle(), &local, 1, &target, 1, 0);

            if (read == -1)
                throw_last_error("process_vm_readv() failed");
            else if(read < size)
                throw_last_error("process_vm_readv() read less than requested");
        }
        template<typename T>
        void read(address_type address, T* buffer, size_type size, std::error_code& ec) const noexcept
        {
            const ::iovec local  = { buffer, size };
            const ::iovec target = { reinterpret_cast<void*>(address + my_base::base_address()), size };

            const auto read = ::process_vm_readv(my_base::handle(), &local, 1, &target, 1, 0);

            if (read == -1)
                ec = get_last_error();
            else if (read < size)
                ec = std::make_error_code(std::errc::result_out_of_range);
        }

        template<typename T>
        void write(address_type address, const T* buffer, size_type size) const
        {
            const ::iovec local  = { buffer, size };
            const ::iovec target = { reinterpret_cast<void*>(address + my_base::base_address()), size };

            const auto written = ::process_vm_writev(my_base::handle(), &local, 1, &target, 1, 0);

            if (written == -1)
                throw_last_error("process_vm_writev() failed");
            else if(written < size)
                throw spm_error(std::error_code(), "process_vm_writev() wrote less than requested");
        }
        template<typename T>
        void write(address_type address, const T* buffer, size_type size, std::error_code& ec) const noexcept
        {
            const ::iovec local  = { buffer, size };
            const ::iovec target = { reinterpret_cast<void*>(address + my_base::base_address()), size };

            const auto written = ::process_vm_writev(my_base::handle(), &local, 1, &target, 1, 0);

            if (written == -1)
                ec = get_last_error();
            else if (written < size)
                ec = std::make_error_code(std::errc::result_out_of_range);
        }
    };

    using memory_traits          = basic_memory_traits<default_rm_storage>;
    using baseless_memory_traits = basic_memory_traits<baseless_rm_storage>;

}}

#endif // include guard
