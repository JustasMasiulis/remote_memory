

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

        }
        template<typename T>
        void read(address_type address, T* buffer, size_type size, std::error_code& ec) const noexcept
        {

        }

        template<typename T>
        void write(address_type address, const T* buffer, size_type size) const
        {

        }
        template<typename T>
        void write(address_type address, const T* buffer, size_type size, std::error_code& ec) const noexcept
        {

        }
    };

    using memory_traits          = basic_memory_traits<default_rm_storage>;
    using baseless_memory_traits = basic_memory_traits<baseless_rm_storage>;

}}

#endif // include guard
