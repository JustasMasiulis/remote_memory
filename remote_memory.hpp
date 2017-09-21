#ifndef REMOTE_MEMORY_HPP
#define REMOTE_MEMORY_HPP

#if defined(JM_PLATFORM_WINDOWS)
    #include "remote_memory/windows
#elif defined(JM_PLATFORM_LINUX)
#elif defined(JM_PLATFORM_OSX)
#else
    #error "unknown platform"
#endif

namespace jm {

    template<class MemoryTraits>
    class basic_remote_memory {

    public:

        template <class T>
        T read();
    };

}

#endif // include guard