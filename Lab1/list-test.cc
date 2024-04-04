
#include "List.h"
#include <iostream>
#include <iterator> //för std::make reverse iterator

#include "catch.hpp"
// kör a.exe --success  för att se mer info
TEST_CASE("Create list")
{
    List lst{1, 4, 2, 6, 8, 9};
    // CHECK(lst.at(2) == 2);
    // CHECK(lst.size() == 6);
    List l2{};
    l2 = lst;
    CHECK(l2.size() == lst.size());
    CHECK(l2.front() == lst.front());
    CHECK(l2.back() == lst.back());
}
// TEST_CASE("List Construction and Basic Access")
// {
//     // Test default constructor
//     List emptyList;
//     CHECK(emptyList.size() == 0);
//     CHECK(emptyList.empty() == true);

//     // Test initializer list constructor
//     List initList{1, 2, 3, 4};
//     CHECK(initList.size() == 4);
//     CHECK(initList.empty() == false);
//     CHECK(initList.front() == 1);
//     CHECK(initList.back() == 4);

//     // Test copy constructor
//     List copiedList = initList;
//     CHECK(copiedList.size() == initList.size());
//     CHECK(copiedList.front() == initList.front());
//     CHECK(copiedList.back() == initList.back());

//     // Test move constructor
//     List movedList = std::move(copiedList);
//     CHECK(movedList.size() == 4);
//     CHECK(copiedList.size() == 0); // Assuming it's cleared
//     CHECK(movedList.front() == 1);
//     CHECK(movedList.back() == 4);
// }
// TEST_CASE("Push Operations")
// {
//     List l3{};

//     // Test push_front
//     l3.push_front(1);
//     l3.push_front(2);
//     CHECK(l3.size() == 2);
//     CHECK(l3.front() == 2);
//     CHECK(l3.back() == 1);

//     // Test push_back
//     l3.push_back(3);
//     l3.push_back(4);
//     CHECK(l3.size() == 4);
//     CHECK(l3.front() == 2);
//     CHECK(l3.back() == 4);
//     std::cout << "l3: ";
// }

// TEST_CASE("Iterator Backwards")
// {
//     List lst{2, 3, 1, 5};
//     auto rb{std::make_reverse_iterator(lst.end())};
//     auto re{std::make_reverse_iterator(lst.begin())};
//     for (auto it = rb; it != re; ++it)
//     {
//         std::cout << *it << ' ';
//     }
// }

// TEST_CASE("Iterator Reverse")
// {
//     List lst{2, 3, 1, 5};
//     auto rb = std::make_reverse_iterator(lst.end());
//     auto re = std::make_reverse_iterator(lst.begin());

//     // Use a vector to collect the elements in reverse order
//     std::vector<int> elements;
//     for (auto it = rb; it != re; ++it)
//     {
//         elements.push_back(*it);
//     }

//     // Now, verify the order of elements is as expected
//     REQUIRE(elements.size() == 4); // Expect 4 elements
//     REQUIRE(elements[0] == 5);     // The first element in reverse order should be 5
//     REQUIRE(elements[1] == 1);     // And so on...
//     REQUIRE(elements[2] == 3);
//     REQUIRE(elements[3] == 2);
// }