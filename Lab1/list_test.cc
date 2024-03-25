#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#include "catch.hpp"
#include "list.h"

using namespace std;

TEST_CASE("Default constructor")
{
  List L{};
}

TEST_CASE("Single Insert")
{
  stringstream ss{};
  List list{};
  list.insert(5);
  ss << list;
  CHECK(ss.str() == "5");
}

TEST_CASE("Multiple Inserts")
{
  stringstream ss{};
  List list{};
  list.insert(5);
  list.insert(6);
  list.insert(7);
  list.insert(-1);
  list.insert(0);
  ss << list;
  CHECK(ss.str() == "-1 0 5 6 7");
}

TEST_CASE("Remove")
{
  stringstream ss{};
  List list{};
  list.insert(5); // 0
  list.insert(6); // 1
  list.insert(7); // 2

  list.remove(1);
  ss << list;
  CHECK(ss.str() == "5 7");

  ss.str("");
  ss.clear();
  list.remove(1);
  ss << list;
  CHECK(ss.str() == "5");

  ss.str("");
  ss.clear();
  list.remove(0);
  ss << list;
  CHECK(ss.str() == "");

  SECTION("Exception")
  {
    List list{};
    list.insert(5);
    list.insert(6);
    list.insert(7);
    CHECK_THROWS(list.remove(6));
  }
}
TEST_CASE("Initializer list")
{
  stringstream ss{};
  List list{1, 3, 2, 5, 4};
  ss << list;
  CHECK(ss.str() == "1 2 3 4 5");
}
// egentligen testas redan detta i remove, då borttagningen inte hade funkat annars, men kan testa igen
TEST_CASE("List size")
{
  List list{};
  CHECK(list.size() == 0);
  list.insert(5);
  CHECK(list.size() == 1);
  list.insert(6);
  CHECK(list.size() == 2);
  list.insert(7);
  CHECK(list.size() == 3);
  list.remove(1);
  CHECK(list.size() == 2);
  list.remove(1);
  CHECK(list.size() == 1);
  list.remove(0);
  CHECK(list.size() == 0);
}

TEST_CASE("Copy constructor")
{
  stringstream ss{};
  List list{};
  list.insert(5);
  list.insert(7);
  list.insert(6);
  List list2{list};
  ss << list2;
  CHECK(ss.str() == "5 6 7");

  SECTION("Copy change")
  {
    stringstream ss{};
    stringstream ss2{};
    List list{5, 6, 10};
    List list2{list};
    list2.insert(7);
    list2.remove(0);
    list.remove(0);
    ss << list;
    ss2 << list2;
    CHECK(ss.str() == "6 10");
    CHECK(ss2.str() == "6 7 10");
  }
}

TEST_CASE("Copy operator")
{
  stringstream ss{};
  List list{5, 6, 10};
  List list2 = list;
  ss << list2;
  CHECK(ss.str() == "5 6 10");
}

TEST_CASE("Move constructor")
{
  stringstream ss{};
  List list{5, 7, 6};
  List list2{move(list)};
  ss << list2;
  CHECK(ss.str() == "5 6 7");
}
TEST_CASE("Move operator")
{
  stringstream ss{};
  List list{5, 7, 6};
  List list2 = move(list);
  ss << list2;
  CHECK(ss.str() == "5 6 7");
}
TEST_CASE("index operator")
{
  List list{5, 6, 7};
  CHECK(list[0] == 5);
  CHECK(list[1] == 6);
  CHECK(list[2] == 7);
  list[0] = 10;
  list[1] = 11;
  list[2] = 12;
  CHECK(list[0] == 10);
  CHECK(list[1] == 11);
  CHECK(list[2] == 12);
  SECTION("Exception")
  {
    List list{1, 2, 3, 4, 5};
    CHECK_THROWS(list[5]);
  }
  SECTION("EMPTY")
  {
    List list{};
    CHECK_THROWS(list[0]);
  }
}
TEST_CASE("List_Iterator")
{
  // både == och = testas i samma test
  SECTION("==")
  {
    List list{5, 6, 7};
    List list2{8, 9, 10};
    List list3{5, 6, 7, 8};
    List::List_iterator itA{list.begin()};
    List::List_iterator itB{list.begin()};
    CHECK(itA == itB);
    List::List_iterator itC{list2.begin()};
    CHECK_FALSE(itA == itC);
    List::List_iterator itD{list3.begin()};
    CHECK_FALSE(itA == itD);
  }

  SECTION("!=")
  {
    List list{5, 6, 7};
    List list2{8, 9, 10};
    List list3{5, 6, 7, 8};
    List::List_iterator itA{list.begin()};
    List::List_iterator itB{list.begin()};
    CHECK_FALSE(itA != itB);
    List::List_iterator itC{list2.begin()};
    CHECK(itA != itC);
    List::List_iterator itD{list3.begin()};
    CHECK(itA != itD);
  }

  SECTION("*")
  {
    List list{5, 6, 7};
    List::List_iterator itA{list.begin()};
    CHECK(*itA == 5);

    List::List_iterator itB{list.end()};
    CHECK_THROWS(*itB); // då end pekar en framför sista elementet, så ska det kasta ett undantag
  }

  SECTION("++")
  {
    List list{5, 6, 7};
    List::List_iterator itA{list.begin()};
    ++itA;
    CHECK(*itA == 6);
    ++itA;
    CHECK(*itA == 7);
    ++itA;
    CHECK_THROWS(++itA); // samma som tidigare, alltså det är här list.end pekar
  }
  SECTION("For-loop")
  {
    stringstream ss{};
    List list{5, 6, 7};
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      ss << *it << " ";
    }
    CHECK(ss.str() == "5 6 7 ");
  }
  SECTION("For-loop empty")
  {
    stringstream ss{};
    List list{};
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      ss << *it << " ";
    }
    CHECK(ss.str() == "");
  }
}
TEST_CASE("Sublist")
{
  stringstream ss{};
  List list{5, 6, 7, 8, 9};
  List sub_list{list.sub({0, 1, 1, 1, 1})};
  ss << sub_list;
  CHECK(ss.str() == "5 6 6 6 6");
  SECTION("More")
  {

    stringstream ss{};

    List list{1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89};
    List sub_list{list.sub({0, 2, 3, 4, 6, 10})}; // [1, 2, 3, 5, 13, 89]
    ss << sub_list;
    CHECK(ss.str() == "1 2 3 5 13 89");
  }
  SECTION("Exception")
  {
    stringstream ss{};
    stringstream ss2{};
    List list{0, 1, 2, 3};
    CHECK_THROWS(list.sub({-1, 5}));
    CHECK_THROWS(list.sub({2, 1, 2}));
  }
}
// fixa sublist och lägga till test för det