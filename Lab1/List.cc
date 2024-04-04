#include "List.h"
#include <iterator>
#include <utility>
#include <stdexcept>
#include <memory>
// blev jobbigt att testa minnesläckor med drmemory, då catch.hpp verisionen som kursen använder (2017) har minnesläckor. hittade nog däremot en läcka som beror på min tail (fungerar kanske ej som en sentinel borde) fråga om detta på labben

List::List()
    // head äger en resurs, tail pekar på en resurs, därför bör head vara en unique_ptr
    : head{std::make_unique<Node>()}, tail(new Node()), sz{}
{
    head->next = std::make_unique<Node>(0, head.get(), nullptr);

    tail = head->next.get();
}

List::List(List const &other)
    : List{}
{
    for (Node *tmp{other.head->next.get()}; tmp != other.tail;)
    {
        push_back(tmp->value);
        tmp = tmp->next.get();
    }
}
List::List(List &&tmp) noexcept
    : List{}
{
    swap(tmp);
}
List::List(std::initializer_list<int> lst)
    : List{}
{
    for (auto val : lst)
    {
        push_back(val);
    }
}

void List::push_front(int value)
{
    std::unique_ptr<Node> old_first{std::move(head->next)};
    head->next = std::make_unique<Node>(value, head.get(), std::move(old_first));

    old_first->prev = head->next.get();
    ++sz;
}
void List::push_back(int value)
{
    Node *old_last = tail->prev;
    old_last->next = std::make_unique<Node>(value, old_last, nullptr);
    tail->prev = old_last->next.get();
    ++sz;
}

bool List::empty() const noexcept
{
    return head->next.get() == tail;
}

int List::back() const noexcept
{
    return tail->prev->value;
}
int &List::back() noexcept
{
    return tail->prev->value;
}

int List::front() const noexcept
{
    return head->next->value;
}
int &List::front() noexcept
{
    return head->next->value;
}

int &List::at(int idx)
{
    return const_cast<int &>(static_cast<List const *>(this)->at(idx));
}
int const &List::at(int idx) const
{
    if (idx >= sz)
        throw std::out_of_range{"Index not found"};
    Node *tmp{head->next.get()};
    while (idx > 0)
    {
        tmp = tmp->next.get();
        --idx;
    }
    return tmp->value;
}

int List::size() const noexcept
{
    return sz;
}

void List::swap(List &other) noexcept
{
    using std::swap;
    swap(head, other.head);
    swap(tail, other.tail);
    swap(sz, other.sz);
}

List &List::operator=(List const &rhs) &
{
    List{rhs}.swap(*this);
    return *this;
}

List &List::operator=(List &&rhs) & noexcept
{
    swap(rhs);
    return *this;
}

// List::List_iterator List::begin() const
// {
//     List_iterator it{};
//     it.pos = head->next.get();
//     return it;
// }

// // end iterator
// List::List_iterator List::end() const
// {
//     List_iterator it{};
//     it.pos = tail;
//     return it;
// }
// List::List_iterator &List::List_iterator::operator++()
// {
//     if (pos != nullptr)
//         pos = pos->next.get();
//     return *this;
// }
// List::List_iterator List::List_iterator::operator++(int)
// {
//     List_iterator temp = *this;
//     ++(*this); // kallar prefix increment
//     return temp;
// }

// // överbelastning för prefix --
// List::List_iterator &List::List_iterator::operator--()
// {
//     if (pos != nullptr)
//         pos = pos->prev;
//     return *this;
// }
// List::List_iterator List::List_iterator::operator--(int)
// {
//     List_iterator temp = *this;
//     --(*this); // kallar prefix decrement
//     return temp;
// }

// List::List_iterator &List::List_iterator::operator=(const List::List_iterator &other)
// {
//     if (this != &other)
//     {
//         pos = other.pos;
//     }
//     return *this;
// }
// // jämförelseoperator
// bool List::List_iterator::operator==(List_iterator const &it) const
// {
//     return pos == it.pos;
// }
// bool List::List_iterator::operator!=(List_iterator const &it) const
// {
//     return pos != it.pos;
// }
// // överbelastning för dereferensoperatorn
// int &List::List_iterator::operator*() const
// {
//     // kan lika gärna göra de här checkarna i begin och end
//     //  if (pos == nullptr)
//     //  {
//     //      throw out_of_range{"Unitiated List_iterator"};
//     //  }
//     //  else if (pos->next == nullptr)
//     //  {
//     //      throw out_of_range{"Index out of range"};
//     //  }
//     //  else
//     //  {
//     return pos->value;
//     // }
// }
// List::List_iterator List::insert(List_iterator position, const int &N)
// {
//     // kolla ifall insert är i början
//     if (position.pos == head->next.get())
//     {
//         push_front(N);
//         return List_iterator{head->next.get()};
//     }

//     // kolla ifall position är i slutt eller oinitierat
//     else if (position.pos == tail || position.pos == nullptr)
//     {
//         push_back(N);
//         // returna det vi nyss inserta
//         return List_iterator{tail->prev};
//     }
//     else
//     {
//         // ny nod
//         auto new_node = std::make_unique<Node>(N);

//         Node *prev_node = position.pos->prev; // säkert eftersom position är inte head->next eller  nullptr
//         new_node->prev = prev_node;
//         new_node->next = std::move(prev_node->next);
//         position.pos->prev = new_node.get();
//         new_node->prev->next = std::move(new_node);

//         return List_iterator{position.pos->prev};
//     }
// }