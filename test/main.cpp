#include <catch_with_main.hpp>
#include <remote_memory.hpp>

const int   integer  = 26;
const float floating = 1.26f;
auto* ptr_i     = &integer;
auto* ptr_ptr_i = &ptr_i;

remote::memory mem;

TEST_CASE("type read<type>(address)")
{
    SECTION("exception based") {
        SECTION("pointer") {
            auto i = mem.read<int>(ptr_i);
            REQUIRE(i == integer);

            auto f = mem.read<float>(&floating);
            REQUIRE(f == floating);

            auto ptr = mem.read<const int*>(&ptr_i);
            REQUIRE(ptr == ptr_i);
        }

        SECTION("integral pointer") {
            auto i = mem.read<int>(reinterpret_cast<std::uintptr_t>(ptr_i));
            REQUIRE(i == integer);

            auto f = mem.read<float>(reinterpret_cast<std::uintptr_t>(&floating));
            REQUIRE(f == floating);

            auto ptr = mem.read<const int*>(reinterpret_cast<std::uintptr_t>(&ptr_i));
            REQUIRE(ptr == ptr_i);
        }
    }

    SECTION("error_code based") {
        std::error_code ec;
        SECTION("pointer") {
            int i = 0;
            REQUIRE_NOTHROW(i = mem.read<int>(ptr_i, ec));
            REQUIRE_FALSE(ec);
            REQUIRE(i == integer);

            float f = 0;
            REQUIRE_NOTHROW(f = mem.read<float>(&floating, ec));
            REQUIRE_FALSE(ec);
            REQUIRE(f == floating);

            const int* ptr = nullptr;
            REQUIRE_NOTHROW(ptr = mem.read<const int*>(&ptr_i, ec));
            REQUIRE_FALSE(ec);
            REQUIRE(ptr == ptr_i);
        }

        SECTION("integral pointer") {
            int i = 0;
            REQUIRE_NOTHROW(i = mem.read<int>(reinterpret_cast<std::uintptr_t>(ptr_i), ec));
            REQUIRE_FALSE(ec);
            REQUIRE(i == integer);

            float f = 0;
            REQUIRE_NOTHROW(f = mem.read<float>(reinterpret_cast<std::uintptr_t>(&floating), ec));
            REQUIRE_FALSE(ec);
            REQUIRE(f == floating);

            const int* ptr = nullptr;
            REQUIRE_NOTHROW(ptr = mem.read<const int*>(reinterpret_cast<std::uintptr_t>(&ptr_i), ec));
            REQUIRE_FALSE(ec);
            REQUIRE(ptr == ptr_i);
        }
    }

}

TEST_CASE("void read(address, T&)")
{
    SECTION("exception based") {
        SECTION("pointer") {
            int i = 0;
            mem.read(ptr_i, i);
            REQUIRE(i == integer);

            float f;
            mem.read(&floating, f);
            REQUIRE(f == floating);

            const int* ptr;
            mem.read(&ptr_i, ptr);
            REQUIRE(ptr == ptr_i);
        }

        SECTION("integral pointer") {
            int i;
            mem.read(reinterpret_cast<std::uintptr_t>(ptr_i), i);
            REQUIRE(i == integer);

            float f;
            mem.read(reinterpret_cast<std::uintptr_t>(&floating), f);
            REQUIRE(f == floating);

            const int* ptr;
            mem.read(reinterpret_cast<std::uintptr_t>(&ptr_i), ptr);
            REQUIRE(ptr == ptr_i);
        }
    }

    SECTION("error code based") {
        std::error_code ec;
        SECTION("pointer") {
            int i = 0;
            mem.read(ptr_i, i, ec);
            REQUIRE_FALSE(ec);
            REQUIRE(i == integer);

            float f;
            mem.read(&floating, f, ec);
            REQUIRE_FALSE(ec);
            REQUIRE(f == floating);

            const int* ptr;
            mem.read<const int*>(&ptr_i, ptr, ec);
            REQUIRE_FALSE(ec);
            REQUIRE(ptr == ptr_i);
        }

        SECTION("integral pointer") {
            int i;
            mem.read(reinterpret_cast<std::uintptr_t>(ptr_i), i, ec);
            REQUIRE_FALSE(ec);
            REQUIRE(i == integer);

            float f;
            mem.read(reinterpret_cast<std::uintptr_t>(&floating), f, ec);
            REQUIRE_FALSE(ec);
            REQUIRE(f == floating);

            const int* ptr;
            mem.read<const int*>(reinterpret_cast<std::uintptr_t>(&ptr_i), ptr, ec);
            REQUIRE_FALSE(ec);
            REQUIRE(ptr == ptr_i);
        }
    }
}

TEST_CASE("void read(address, T*, size)")
{
    SECTION("exception based") {
        SECTION("pointer") {
            int i = 0;
            mem.read(ptr_i, &i, sizeof(i));
            REQUIRE(i == integer);

            float f;
            mem.read(&floating, &f, sizeof(f));
            REQUIRE(f == floating);

            const int* ptr;
            mem.read(&ptr_i, &ptr, sizeof(ptr));
            REQUIRE(ptr == ptr_i);
        }

        SECTION("integral pointer") {
            int i;
            mem.read(reinterpret_cast<std::uintptr_t>(ptr_i), &i, sizeof(i));
            REQUIRE(i == integer);

            float f;
            mem.read(reinterpret_cast<std::uintptr_t>(&floating), &f, sizeof(f));
            REQUIRE(f == floating);

            const int* ptr;
            mem.read(reinterpret_cast<std::uintptr_t>(&ptr_i), &ptr, sizeof(ptr));
            REQUIRE(ptr == ptr_i);
        }
    }

    SECTION("error code based") {
        std::error_code ec;
        SECTION("pointer") {
            int i = 0;
            mem.read(ptr_i, &i, sizeof(i), ec);
            REQUIRE_FALSE(ec);
            REQUIRE(i == integer);

            float f;
            mem.read(&floating, &f, sizeof(f), ec);
            REQUIRE_FALSE(ec);
            REQUIRE(f == floating);

            const int* ptr;
            mem.read<const int*>(&ptr_i, &ptr, sizeof(ptr), ec);
            REQUIRE_FALSE(ec);
            REQUIRE(ptr == ptr_i);
        }

        SECTION("integral pointer") {
            int i;
            mem.read(reinterpret_cast<std::uintptr_t>(ptr_i), &i, sizeof(i), ec);
            REQUIRE_FALSE(ec);
            REQUIRE(i == integer);

            float f;
            mem.read(reinterpret_cast<std::uintptr_t>(&floating), &f, sizeof(f), ec);
            REQUIRE_FALSE(ec);
            REQUIRE(f == floating);

            const int* ptr;
            mem.read<const int*>(reinterpret_cast<std::uintptr_t>(&ptr_i), &ptr, sizeof(ptr), ec);
            REQUIRE_FALSE(ec);
            REQUIRE(ptr == ptr_i);
        }
    }
}

TEST_CASE("void write(address, const T&)")
{
    SECTION("exception based") {
        SECTION("pointer") {
            int i_rv = 0;
            mem.write(&i_rv, 69);
            REQUIRE(i_rv == 69);

            int i = 0;
            mem.write(&i, integer);
            REQUIRE(i == integer);

            float f;
            mem.write(&f, floating);
            REQUIRE(f == floating);

            const int* ptr;
            mem.write(&ptr, ptr_i);
            REQUIRE(ptr == ptr_i);
        }

        SECTION("integral pointer") {
            int i = 0;
            mem.write(reinterpret_cast<std::uintptr_t>(&i), integer);
            REQUIRE(i == integer);

            float f;
            mem.write(reinterpret_cast<std::uintptr_t>(&f), floating);
            REQUIRE(f == floating);

            const int* ptr;
            mem.write(reinterpret_cast<std::uintptr_t>(&ptr), ptr_i);
            REQUIRE(ptr == ptr_i);
        }
    }

    SECTION("error code based") {
        std::error_code ec;
        SECTION("pointer") {
            int i_rv = 0;
            mem.write(&i_rv, 69, ec);
            REQUIRE_FALSE(ec);
            REQUIRE(i_rv == 69);

            int i = 0;
            mem.write(&i, integer, ec);
            REQUIRE_FALSE(ec);
            REQUIRE(i == integer);

            float f;
            mem.write(&f, floating, ec);
            REQUIRE_FALSE(ec);
            REQUIRE(f == floating);

            const int* ptr;
            mem.write<const int*>(&ptr, ptr_i, ec);
            REQUIRE_FALSE(ec);
            REQUIRE(ptr == ptr_i);
        }

        SECTION("integral pointer") {
            int i = 0;
            mem.write(reinterpret_cast<std::uintptr_t>(&i), integer, ec);
            REQUIRE_FALSE(ec);
            REQUIRE(i == integer);

            float f;
            mem.write(reinterpret_cast<std::uintptr_t>(&f), floating, ec);
            REQUIRE_FALSE(ec);
            REQUIRE(f == floating);

            const int* ptr;
            mem.write<const int*>(reinterpret_cast<std::uintptr_t>(&ptr), ptr_i, ec);
            REQUIRE_FALSE(ec);
            REQUIRE(ptr == ptr_i);
        }
    }
}

TEST_CASE("void write(address, const T*, size)")
{
    SECTION("exception based") {
        SECTION("pointer") {
            int i = 0;
            mem.write(&i, &integer, sizeof(int));
            REQUIRE(i == integer);

            float f;
            mem.write(&f, &floating, sizeof(float));
            REQUIRE(f == floating);

            const int* ptr;
            mem.write(&ptr, &ptr_i, sizeof(ptr_i));
            REQUIRE(ptr == ptr_i);
        }

        SECTION("integral pointer") {
            int i = 0;
            mem.write(reinterpret_cast<std::uintptr_t>(&i), &integer, sizeof(int));
            REQUIRE(i == integer);

            float f;
            mem.write(reinterpret_cast<std::uintptr_t>(&f), &floating, sizeof(float));
            REQUIRE(f == floating);

            const int* ptr;
            mem.write(reinterpret_cast<std::uintptr_t>(&ptr), &ptr_i, sizeof(ptr_i));
            REQUIRE(ptr == ptr_i);
        }
    }

    SECTION("error code based") {
        std::error_code ec;
        SECTION("pointer") {
            int i = 0;
            mem.write(&i, &integer, sizeof(int), ec);
            REQUIRE_FALSE(ec);
            REQUIRE(i == integer);

            float f;
            mem.write(&f, &floating, sizeof(float), ec);
            REQUIRE_FALSE(ec);
            REQUIRE(f == floating);

            const int* ptr;
            mem.write<const int*>(&ptr, &ptr_i, sizeof(ptr_i), ec);
            REQUIRE_FALSE(ec);
            REQUIRE(ptr == ptr_i);
        }

        SECTION("integral pointer") {
            int i = 0;
            mem.write(reinterpret_cast<std::uintptr_t>(&i), &integer, sizeof(int), ec);
            REQUIRE_FALSE(ec);
            REQUIRE(i == integer);

            float f;
            mem.write(reinterpret_cast<std::uintptr_t>(&f), &floating, sizeof(float), ec);
            REQUIRE_FALSE(ec);
            REQUIRE(f == floating);

            const int* ptr;
            mem.write<const int*>(reinterpret_cast<std::uintptr_t>(&ptr), &ptr_i, sizeof(ptr_i), ec);
            REQUIRE_FALSE(ec);
            REQUIRE(ptr == ptr_i);
        }
    }
}

TEST_CASE("traverse_pointers_chain")
{
    struct fake_data {
        fake_data(std::uint64_t i_, fake_data* ptr)
                : i(i_), p(ptr) {}
        std::uint64_t i;
        fake_data* p;
    };
    static_assert(sizeof(fake_data) == sizeof(void*) + 8, "SPENT LIKE 4 HOURS ON THIS THING TURNS OUT IT PADDED THE 32 BIT INT");

    auto data = new fake_data(333, new fake_data(222, new fake_data(111, nullptr)));

    auto result = mem.read<std::uint64_t>(mem.traverse_pointers_chain(reinterpret_cast<std::uintptr_t>(data) + 8, 8));

    REQUIRE(result == 111 );
}