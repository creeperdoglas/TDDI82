#include "List.h"
#include <iterator>
#include <utility>
#include <stdexcept>

struct List::Node
{
    Node() = default;
    Node(int v, Node* p, Node* n)
        : value{v}, prev{p}, next{n} {}
    int value {};
    Node * prev {};
    Node * next {};
};

List::List()
    : head{ new Node{} }, tail{}, sz{}
{
    head->next = new Node{0, head, nullptr};
    tail = head->next;
}

List::List(List const & other)
    : List{}
{
    for (Node * tmp {other.head->next}; tmp != other.tail ; )
    {
        push_back(tmp->value);
        tmp = tmp->next;
    }
}
List::List(List && tmp) noexcept
    :List{}
{
    swap(tmp);
}
List::List(std::initializer_list<int> lst)
    : List{}
{
    for ( auto val : lst )
    {
        push_back(val);
    }
}

void List::push_front(int value)
{
    Node * old_first { head->next };
    head->next = new Node{value, head, head->next};
    old_first->prev = head->next;
    ++sz;
}
void List::push_back(int value)
{
    Node * old_last { tail->prev };
    old_last->next = new Node{value, old_last, tail};
    tail->prev = old_last->next;
    ++sz;
}

bool List::empty() const noexcept
{
    return head->next == tail;
}

int List::back() const noexcept
{
    return tail->prev->value;
}
int & List::back() noexcept
{
    return tail->prev->value;
}

int List::front() const noexcept
{
    return head->next->value;
}
int & List::front() noexcept
{
    return head->next->value;
}

int & List::at(int idx)
{
    return const_cast<int &>(static_cast<List const *>(this)->at(idx));
}
int const & List::at(int idx) const
{
    if (idx >= sz)
        throw std::out_of_range{"Index not found"};
    Node * tmp {head->next};
    while ( idx > 0 )
    {
        tmp = tmp->next;
        --idx;
    }
    return tmp->value;
}

int List::size() const noexcept
{
    return sz;
}

void List::swap(List & other) noexcept
{
    using std::swap;
    swap(head, other.head);
    swap(tail, other.tail);
    swap(sz, other.sz);
}

List & List::operator=(List const & rhs) &
{
    List{rhs}.swap(*this);
    return *this;
}

List & List::operator=(List && rhs)& noexcept
{
    swap(rhs);
    return *this;
}
