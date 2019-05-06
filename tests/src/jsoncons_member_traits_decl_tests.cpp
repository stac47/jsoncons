// Copyright 2013 Daniel Parker
// Distributed under Boost license

#if defined(_MSC_VER)
#include "windows.h" // test no inadvertant macro expansions
#endif
#include <jsoncons/json.hpp>
#include <jsoncons/json_encoder.hpp>
#include <catch/catch.hpp>
#include <sstream>
#include <vector>
#include <utility>
#include <ctime>
#include <cstdint>

using namespace jsoncons;

namespace jsoncons_member_traits_decl_tests {

    struct book
    {
        std::string author;
        std::string title;
        double price;
    };
    struct book2
    {
        std::string author;
        std::string title;
        double price;
        std::string isbn;
    };
    class book3
    {
        std::string author_;
        std::string title_;
        double price_;
    public:
        book3(const std::string& author)
            : author_(author), title_(""), price_(10)
        {
        }

        book3(const std::string& author,
              const std::string& title,
              double price)
            : author_(author), title_(title), price_(price)
        {
        }

        book3(const book3&) = default;
        book3(book3&&) = default;
        book3& operator=(const book3&) = default;
        book3& operator=(book3&&) = default;

        const std::string& author() const
        {
            return author_;
        }

        const std::string& title() const
        {
            return title_;
        }

        double price() const
        {
            return price_;
        }
    };

} // namespace jsoncons_member_traits_decl_tests
 
namespace ns = jsoncons_member_traits_decl_tests;

JSONCONS_MEMBER_TRAITS_DECL(ns::book,author,title,price)
JSONCONS_MEMBER_TRAITS_DECL(ns::book2,author,title,price,isbn)
JSONCONS_ACONS_TRAITS_DECL(ns::book3,author,title,price)


TEST_CASE("JSONCONS_MEMBER_TRAITS_DECL tests")
{
    std::string author = "Haruki Murakami"; 
    std::string title = "Kafka on the Shore";
    double price = 25.17;

    ns::book book{author, title, price};

    SECTION("book")
    {
        std::string s;

        encode_json(book, s);

        json j = decode_json<json>(s);

        REQUIRE(j.is<ns::book>() == true);
        REQUIRE(j.is<ns::book2>() == false);

        CHECK(j["author"].as<std::string>() == author);
        CHECK(j["title"].as<std::string>() == title);
        CHECK(j["price"].as<double>() == Approx(price).epsilon(0.001));

        json j2(book);

        std::cout << pretty_print(j2) << "\n";

        CHECK(j == j2);

        ns::book val = j.as<ns::book>();

        CHECK(val.author == book.author);
        CHECK(val.title == book.title);
        CHECK(val.price == Approx(book.price).epsilon(0.001));
    }
}

TEST_CASE("JSONCONS_ACONS_TRAITS_DECL tests")
{
    std::string an_author = "Haruki Murakami"; 
    std::string a_title = "Kafka on the Shore";
    double a_price = 25.17;

    SECTION("is")
    {
        json j;
        j["author"] = an_author;

        bool val = true;

        JSONCONS_IS2(1,j,ns::book3,val,author);
        CHECK(val == true);
    }
    SECTION("is 2")
    {
        json j;
        j["author"] = an_author;
        j["title"] = a_title;
        j["price"] = a_price;

        bool val = j.is<ns::book3>();
        CHECK(val == true);
    }
    SECTION("to_json")
    {
        ns::book3 book(an_author,a_title,a_price);

        json j;

        JSONCONS_TO_JSON2(1,j,ns::book3,book,author);
        CHECK(j["author"].as<std::string>() == an_author);
    }
    SECTION("to_json (2)")
    {
        ns::book3 book(an_author,a_title,a_price);

        json j(book);

        std::cout << pretty_print(j) << "\n";

        CHECK(j["author"].as<std::string>() == an_author);
        CHECK(j["title"].as<std::string>() == a_title);
        CHECK(j["price"].as<double>() == Approx(a_price).epsilon(0.001));
    }
    SECTION("as")
    {
        json j;
        j["author"] = an_author;
        j["title"] = a_title;
        j["price"] = a_price;
 
        ns::book3 book(
            JSONCONS_AS2(1,j,ns::book3,void(),author)
            JSONCONS_AS2(1,j,ns::book3,void(),title)
            JSONCONS_AS2_LAST(1,j,ns::book3,void(),price)
        );

        CHECK(book.author() == an_author);
        CHECK(book.title() == a_title);
        CHECK(book.price() == Approx(a_price).epsilon(0.001));
    }
    SECTION("as 2")
    {
        json j;
        j["author"] = an_author;
        j["title"] = a_title;
        j["price"] = a_price;

        ns::book3 book = j.as<ns::book3>();

        CHECK(book.author() == an_author);
        CHECK(book.title() == a_title);
        CHECK(book.price() == Approx(a_price).epsilon(0.001));
    }
}
