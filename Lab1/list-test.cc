
#include "List.h"
#include "iostream"

#include "catch.hpp"

// Skapar en lista med element och kontrollerar tillgång via index och listans storlek
TEST_CASE("Create list")
{
    List_NS::List<int> lst{1, 4, 2, 6, 8, 9};
    CHECK(lst.at(2) == 2);
    CHECK(lst.size() == 6);
    List_NS::List<int> l2;
    l2 = lst;
    CHECK(l2.size() == lst.size());
    CHECK(l2.front() == lst.front());
    CHECK(l2.back() == lst.back());
}

// Testar funktionen för att byta innehållet mellan två listor
TEST_CASE("Swap")
{
    List_NS::List<int> l1{1, 4, 2, 6, 8, 9};
    List_NS::List<int> l2{5, 9, 1, 4, 6};
    l2.swap(l1);
    CHECK(l1.at(2) == 1);
    CHECK(l2.at(4) == 8);
    CHECK(l1.size() == 5);
    CHECK(l2.size() == 6);
}

// Lägger till element i början och slutet av listan och verifierar listans storlek och innehåll
TEST_CASE("Push front and push back")
{
    List_NS::List<int> l1{1, 4, 2, 6, 8, 9};
    l1.push_front(4);
    CHECK(l1.at(0) == 4);
    CHECK(l1.size() == 7);
    l1.push_back(5);
    CHECK(l1.at(7) == 5);
    CHECK(l1.size() == 8);
}

// Går igenom listan baklänges med en omvänd iterator, den från labb instruktioner
TEST_CASE("Reverse Iterator")
{
    List_NS::List<int> l1{5, 4, 3, 7, 1};
    auto rb{std::make_reverse_iterator(l1.end())};
    auto re{std::make_reverse_iterator(l1.begin())};

    int size{4};
    for (auto it{rb}; it != re; ++it)
    {
        CHECK(*it == l1.at(size));
        --size;
    }
}

TEST_CASE("Iterator")
{
    List_NS::List<int> l1{1, 7, 3, 4, 5};
    auto rb{l1.begin()};
    auto re{l1.end()};

    int size{0};
    for (auto it{rb}; it != re; ++it)
    {
        CHECK(*it == l1.at(size));
        ++size;
    }
}

TEST_CASE("Iterator preincrement")
{
    List_NS::List<int> l1{1, 5, 3, 8};
    auto it1{l1.begin()};
    auto it2{it1};

    it2 = ++it1;
    CHECK(*it1 == 5);
    CHECK(*it2 == 5);

    it2 = ++it1;
    CHECK(*it1 == 3);
    CHECK(*it2 == 3);
}

TEST_CASE("Iterator postincrement")
{
    List_NS::List<int> l1{1, 5, 3, 8};
    auto it1{l1.begin()};
    auto it2{it1};

    CHECK(*(it1++) == 1);
    CHECK(*(it2++) == 1);

    CHECK(*(it1++) == 5);
    CHECK(*(it2++) == 5);
}

TEST_CASE("Iterator predecrement")
{
    List_NS::List<int> l1{1, 5, 3, 8};
    auto it1{l1.end()};

    CHECK(*it1 == 0);
    --it1;
    CHECK(*it1 == 8);

    CHECK(*it1 == 8);
    --it1;
    CHECK(*it1 == 3);
}

TEST_CASE("Iterator postdecrement")
{
    List_NS::List<int> l1{1, 5, 3, 8};
    auto it1{l1.end()};

    CHECK(*it1 == 0);
    CHECK(*(it1--) == 0);

    CHECK(*it1 == 8);
    CHECK(*(it1--) == 8);
}

TEST_CASE("Iterator operator ==")
{
    List_NS::List<int> l1{1, 5, 3, 8};
    auto rb{l1.begin()};
    auto re{l1.end()};

    auto it1{rb};
    auto it2{rb};

    CHECK(*it1 == *it2);
}

TEST_CASE("Iterator operator !=")
{
    List_NS::List<int> l1{1, 5, 3, 8};
    auto rb{l1.begin()};
    auto re{l1.end()};

    auto it1{rb};
    auto it2{re};

    CHECK(*it1 != *it2);
}

TEST_CASE("Iterator operator ->")
{
    List_NS::List<int> l1{1, 5, 3, 8};
    List_NS::List<std::string> l2{"hello", "hola"};
    auto rb = l1.begin();
    auto sb = l2.begin();

    CHECK(*rb == 1);
    ++rb;
    CHECK(*rb == 5);

    CHECK(*sb == "hello");
    ++sb;
    CHECK(*sb == "hola");
}

TEST_CASE("Other datatypes")
{
    List_NS::List<std::string> l1{"hola", "hello"};
    CHECK(l1.at(0) == "hola");
    CHECK(l1.at(1) == "hello");
    CHECK(l1.size() == 2);

    List_NS::List<char> l2{'a', 'b', 'c'};
    CHECK(l2.at(1) == 'b');
    CHECK(l2.size() == 3);
}

class List
{
};

TEST_CASE("Namespaces")
{
    List l1;
    List_NS::List<int> l2;
}