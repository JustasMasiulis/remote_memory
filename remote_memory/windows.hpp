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
    class default_storage
    {
        typename TraitsType::address_type _base_address;
    public:
        const AddressType& base_address() const noexcept { return _base_address; }
        AddressType& base_address() noexcept { return _base_address; }

        void* handle() const noexcept {}
    };

    template<template<class> class Storage>
    struct basic_memory_traits : public Storage<basic_memory_traits> {
        using address_type = std::uintptr_t;
        using size_type    = SIZE_T_;

    protected:
        template<typename T>
        void read(address_type address, T* buffer, size_type size)
        {
            if (!ReadProcessMemory(Storage::handle()
                                   , reinterpret_cast<const void*>(address + Storage::base_address())
                                   , buffer
                                   , size
                                   , nullptr))
                throw_last_error("ReadProcessMemory() failed");
        }

        template<typename T>
        void write(address_type address, const T* buffer, size_type size)
        {
            if (!WriteProcessMemory(Storage::handle()
                                    , reinterpret_cast<void*>(address + Storage::base_address())
                                    , buffer
                                    , size
                                    , nullptr))
                throw_last_error("WriteProcessMemory() failed");
        }
    };

}}