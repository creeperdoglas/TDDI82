
#include "List.h"
#include <iostream>
#include <iterator> //för std::make reverse iterator

#include "catch.hpp"
// kör a.exe --success  för att se mer info
TEST_CASE("Create list")
{
    List lst{1, 4, 2, 6, 8, 9};
    CHECK(lst.at(2) == 2);
    CHECK(lst.size() == 6);
    List l2;
    l2 = lst;
    CHECK(l2.size() == lst.size());
    CHECK(l2.front() == lst.front());
    CHECK(l2.back() == lst.back());
}

// TEST_CASE("Iterator Backwards")
// {
//     List lst{2, 3, 1, 5};
//     auto rb{make_reverse_iterator(lst.end())};
//     auto re{make_reverse_iterator(lst.begin())};
//     for (auto it = rb; it != re; ++it)
//     {
//         cout << *it << ' ';
//     }
// }

TEST_CASE("Iterator Reverse")
{
    List lst{2, 3, 1, 5};
    auto rb = std::make_reverse_iterator(lst.end());
    auto re = std::make_reverse_iterator(lst.begin());

    // Use a vector to collect the elements in reverse order
    std::vector<int> elements;
    for (auto it = rb; it != re; ++it)
    {
        elements.push_back(*it);
    }

    // Now, verify the order of elements is as expected
    REQUIRE(elements.size() == 4); // Expect 4 elements
    REQUIRE(elements[0] == 5);     // The first element in reverse order should be 5
    REQUIRE(elements[1] == 1);     // And so on...
    REQUIRE(elements[2] == 3);
    REQUIRE(elements[3] == 2);
}