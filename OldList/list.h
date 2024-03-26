#pragma once
#include <iostream>
#include <initializer_list>
#include <string>
#include <memory>

class List
{
private:
  class Element
  {
  public:
    int value;
    Element *next;
    Element *prev;

    Element(int n = 0) : value(n), next(nullptr), prev(nullptr) {}
  };

  Element *first, *last;

public:
  List();
  List(std::initializer_list<int> const &data);
  List(List const &L);
  List(List &&L);
  ~List();

  List &operator=(List const &L);
  List &operator=(List &&L) noexcept;

  void insert(int const N);
  void remove(int const N);
  int size() const;
  void append(int N);
  std::string to_string() const;
  int &operator[](int const index) const;

  class List_iterator
  {
  public:
    friend class List;
    List_iterator(const List_iterator &) = default;
    List_iterator &operator=(List_iterator const &);
    List_iterator &operator++();
    List_iterator &operator--();
    bool operator==(List_iterator const &it) const;
    bool operator!=(List_iterator const &it) const;
    int &operator*() const;
    explicit List_iterator(Element *p = nullptr) : pos(p) {}

  private:
    // List_iterator() = default;
    Element *pos; // list_iterator(element *p) var det som tidigare stod, ta en titt senare. detta är en variabel istället för funktion borde bli bättre
  };
  List_iterator begin() const;
  List_iterator end() const;
  List_iterator insert(List_iterator position, const int &N);
  List sub(std::initializer_list<int> indices);
};
std::ostream &operator<<(std::ostream &os, const List &list);

// struct List::Node
// {
//   Node() = default;
//   Node(int v, std::unique_ptr<Node> p, std::unique_ptr<Node> n)
//       : value{v}, prev{std::move(p)}, next{std::move(n)} {}
//   int value{};
//   std::unique_ptr<Node> prev{};
//   std::unique_ptr<Node> next{};
// };

// List::List()
//     : head{std::make_unique<Node>()}, tail{}, sz{}
// {
//   head->next = std::make_unique<Node>(0, std::move(head), nullptr);
//   tail = std::move(head->next);
// }

// void List::push_front(int value)
// {
//   std::unique_ptr<Node> old_first = std::move(head->next);
//   head->next = std::make_unique<Node>(value, std::move(head), std::move(old_first));
//   old_first->prev = std::move(head->next);
//   ++sz;
// }

// void List::push_back(int value)
// {
//   std::unique_ptr<Node> old_last = std::move(tail->prev);
//   old_last->next = std::make_unique<Node>(value, std::move(old_last), std::move(tail));
//   tail->prev = std::move(old_last->next);
//   ++sz;
// }