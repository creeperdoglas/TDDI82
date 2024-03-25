#pragma once
#include <iostream>
#include <initializer_list>
#include <string>

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
