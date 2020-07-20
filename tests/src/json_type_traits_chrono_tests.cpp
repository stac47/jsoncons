// Copyright 2020 Daniel Parker
// Distributed under Boost license

#include <catch/catch.hpp>
#include <jsoncons/json.hpp>

using namespace jsoncons;

#if 0
namespace ns {

    template <class T>
    struct A
    {
        static void f()
        {
            static_assert(sizeof(T) > 0, "f not implemented");
        }
    };

    template<class T>
    struct A<std::chrono::duration<T>>
    {
        static void f()
        {
            std::cout << "Hello duration\n";
        }
    };
/*
    template <>
    struct A<std::chrono::seconds>
    {
        static void f()
        {
            std::cout << "Hello seconds\n";
        }
    };
*/
    template <>
    struct A<std::string>
    {
        static void f()
        {
            std::cout << "Hello string\n";
        }
    };
}

TEST_CASE("test_chrono")
{
    ns::A<std::chrono::seconds>::f();
    ns::A<std::string>::f();
}
#endif 

TEST_CASE("json_type_traits chron tests")
{
    SECTION("test 1")
    {
        uint64_t time = 1000;

        json j(time, semantic_tag::epoch_time);

        auto val = j.as<std::chrono::seconds>(); 

        CHECK(val.count() == std::chrono::seconds::rep(time));
    }
    SECTION("test 2")
    {
        double time = 1000.100;

        json j(time, semantic_tag::epoch_time);

        auto val = j.as<std::chrono::duration<double>>();

        CHECK(val.count() == time);
    }
}