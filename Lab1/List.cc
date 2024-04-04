#include "List.h"
#include <iterator>
#include <utility>
#include <stdexcept>
#include <memory>

/*struct List::Node
{
    Node() = default;
    Node(int v, Node* p, Node* n)
        : value{v}, prev{p}, next{n} {}
    int value {};
    Node* prev {};
    std::unique_ptr<List::Node> next {};
};*/

List::List()
    : head{std::make_unique<Node>()}, tail{head.get()}, sz{}
{
    // head->next = new Node{0, head, nullptr};
    // tail = head->next;
    // tail = head.get();
}

List::List(List const &other)
    : List{}
{
    for (Node *tmp{other.head.get()}; tmp != other.tail;)
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
    // Node * old_first { head->next.get() };
    head = std::make_unique<Node>(value, nullptr, head.release());
    head.get()->next.get()->prev = head.get();
    // old_first->prev = head->next.get();
    if (sz == 0)
    {
        tail->prev = head.get();
    }

    ++sz;
}
void List::push_back(int value)
{
    /*Node * old_last { tail->prev };
    old_last->next = std::make_unique<Node>(value, old_last, tail);
    tail->prev = old_last->next.get();*/
    if (empty())
    {
        push_front(value);
    }
    else
    {
        tail->prev->next.release();
        tail->prev->next = std::make_unique<Node>(value, tail->prev, tail);
        tail->prev = tail->prev->next.get();
        ++sz;
    }
}

bool List::empty() const noexcept
{
    return head.get() == tail;
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
    return head->value;
}
int &List::front() noexcept
{
    return head->value;
}

int &List::at(int idx)
{
    return const_cast<int &>(static_cast<List const &>(*this).at(idx));
}
int const &List::at(int idx) const
{
    if (idx >= sz)
        throw std::out_of_range{"Index not found"};
    Node *tmp{head.get()};
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

// iterator

List::List_Iterator::List_Iterator(Node *ptr)
    : curr(ptr) {}

List::List_Iterator::reference List::List_Iterator::operator*() const
{
    return curr->value;
}

List::List_Iterator &List::List_Iterator::operator++()
{
    if (curr->next != nullptr)
    {
        curr = curr->next.get();
        return *this;
    }
}

List::List_Iterator List::List_Iterator::operator++(int)
{
    List_Iterator tmp(*this);
    if (curr->next != nullptr)
    {
        curr = curr->next.get();
        return tmp;
    }
}

List::List_Iterator &List::List_Iterator::operator--()
{
    if (curr->prev != nullptr)
    {
        curr = curr->prev;
        return *this;
    }
}

List::List_Iterator List::List_Iterator::operator--(int)
{
    List_Iterator tmp(*this);
    if (curr->prev != nullptr)
    {
        curr = curr->prev;
        return tmp;
    }
}

bool List::List_Iterator::operator==(const List_Iterator &rhs) const
{
    return curr == rhs.curr;
}

bool List::List_Iterator::operator!=(const List_Iterator &rhs) const
{
    return curr != rhs.curr;
}

List::List_Iterator List::begin() const
{
    return List_Iterator(head.get());
}

List::List_Iterator List::end() const
{
    return List_Iterator(tail);
}