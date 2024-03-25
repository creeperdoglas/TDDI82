#include "list.h"
#include <stdexcept>
#include <vector>
#include <utility>
#include <sstream>
using namespace std;
// standradkonstruktor
List::List() : first(new Element()), last(new Element())
{
  first->next = last;
  last->prev = first;
}

// Konstruktor som tar en initializer_list som argument och skapar en lista med elementen i listan.
List::List(std::initializer_list<int> const &data) : List()
{
  last->prev = first;
  first->next = last;
  for (auto &i : data)
  {
    insert(i);
  }
}

// Copy constructor
List::List(List const &L) : List()
{
  last->prev = first;
  first->next = last;
  Element *temp{L.first};
  while (temp->next != L.last)
  {
    insert(temp->next->value);
    temp = temp->next;
  }
}

// Move constructor
List::List(List &&L) : first(std::exchange(L.first, nullptr)), last(std::exchange(L.last, nullptr)) {}

// Destructor
List::~List()
{
  if (first != nullptr && last != nullptr)
  {
    Element *temp{first->next};
    while (temp != last)
    {
      temp = temp->next;
      delete temp->prev;
    }
    delete first;
    delete last;
  }
}

// Copy assignment operator
List &List::operator=(List const &L)
{
  if (this != &L)
  {
    Element *temp{L.first};
    while (temp->next != L.last)
    {
      insert(temp->next->value);
      temp = temp->next;
    }
  }
  return *this;
}

// Move assignment operator
List &List::operator=(List &&L) noexcept
{
  if (this != &L)
  {
    last = exchange(L.last, nullptr);
    first = exchange(L.first, nullptr);
  }
  return *this;
}

// Insert an element i sorterad ordning
void List::insert(int N)
{
  Element *temp{first->next};
  Element *new_element{new Element{N}};
  while (temp != last && temp->value <= N)
  {
    temp = temp->next;
  }
  new_element->next = temp;
  new_element->prev = temp->prev;
  temp->prev->next = new_element;
  temp->prev = new_element;
}

// Remove
void List::remove(int N)
{
  Element *temp{first};
  for (int i{}; i < N + 1; i++)
  {
    temp = temp->next;
    if (temp == last)
    {
      throw out_of_range{"Index out of range"};
    }
  }
  temp->next->prev = temp->prev;
  temp->prev->next = temp->next;
  delete temp;
}

// Returnerar en referens till det element som finns på platsen index i listan.
int List::size() const
{
  int size{};
  Element *temp{first};
  while (temp->next != last)
  {
    size++;
    temp = temp->next;
  }
  return size;
}

// överbelastad operator som returnerar en referens till det element som finns på platsen index i listan.
int &List::operator[](int const index) const
{
  if (index < 0 || index >= size())
  {
    throw out_of_range{"Index out of range"};
  }
  Element *temp{first};
  for (int i{}; i <= index; i++)
  {
    temp = temp->next;
  }
  return temp->value;
}

// konvertera lista till string
std::string List::to_string() const
{
  std::stringstream ss;
  for (Element *curr = first->next; curr != last; curr = curr->next)
  {
    ss << curr->value;
    if (curr->next != last)
      ss << " ";
  }
  return ss.str();
}
// List::Element::Element(int n) : value(n), next(nullptr), prev(nullptr) {}

// Overload << operator
std::ostream &operator<<(std::ostream &os, List const &L)
{
  os << L.to_string();
  return os;
}
// begin iterator
List::List_iterator List::begin() const
{
  List_iterator it{};
  it.pos = first->next;
  return it;
}

// end iterator
List::List_iterator List::end() const
{
  List_iterator it{};
  it.pos = last;
  return it;
}
// överbelastning för prefix ++
List::List_iterator &List::List_iterator::operator++()
{
  if (pos == nullptr)
  {
    throw out_of_range{"Unitiated List_iterator"};
  }
  else if (pos->next == nullptr)
  {
    throw out_of_range{"Index out of range"};
  }
  else
  {
    pos = pos->next;
    return *this;
  }
}
// överbelastning för prefix --
List::List_iterator &List::List_iterator::operator--()
{
  if (pos == nullptr)
  {
    throw out_of_range{"Unitiated List_iterator"};
  }
  else if (pos->prev == nullptr)
  {
    throw out_of_range{"Index out of range"};
  }
  else
  {
    pos = pos->prev;
    return *this;
  }
}
List::List_iterator &List::List_iterator::operator=(const List::List_iterator &other)
{
  if (this != &other)
  {
    pos = other.pos;
  }
  return *this;
}
// jämförelseoperator
bool List::List_iterator::operator==(List_iterator const &it) const
{
  return pos == it.pos;
}
bool List::List_iterator::operator!=(List_iterator const &it) const
{
  return pos != it.pos;
}
// överbelastning för dereferensoperatorn
int &List::List_iterator::operator*() const
{
  if (pos == nullptr)
  {
    throw out_of_range{"Unitiated List_iterator"};
  }
  else if (pos->next == nullptr)
  {
    throw out_of_range{"Index out of range"};
  }
  else
  {
    return pos->value;
  }
}
List::List_iterator List::insert(List_iterator position, const int &N)
{
  Element *temp = position.pos;
  Element *new_element = new Element{N};
  while (temp != last && temp->value <= N)
  {
    temp = temp->next;
  }
  new_element->next = temp;
  new_element->prev = temp->prev;
  temp->prev->next = new_element;
  temp->prev = new_element;
  return List_iterator{new_element};
}

// Append
void List::append(int N)
{
  Element *newElement = new Element{N};
  newElement->next = last;
  newElement->prev = last->prev;
  last->prev->next = newElement;
  last->prev = newElement;
}

// ...
List List::sub(std::initializer_list<int> indices)
{
  List sublist;
  int prev_index = -1;
  for (const auto &index : indices)
  {
    if (index < 0 || index >= size())
    {
      throw out_of_range("Index out of range");
    }
    if (index < prev_index)
    {
      throw invalid_argument("Indices not in ascending order");
    }
    sublist.append((*this)[index]); // istället för insert
    prev_index = index;
  }
  return sublist;
}