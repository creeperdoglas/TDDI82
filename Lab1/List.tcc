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

template <typename T>
List_NS::List<T>::List()
    : head{std::make_unique<Node>()}, tail{head.get()}, sz{}
{
    // head->next = new Node{0, head, nullptr};
    // tail = head->next;
    // tail = head.get();
}

template <typename T>
List_NS::List<T>::List(List const &other)
    : List{}
{
    for (Node *tmp{other.head.get()}; tmp != other.tail;)
    {
        push_back(tmp->value);
        tmp = tmp->next.get();
    }
}

template <typename T>
List_NS::List<T>::List(List &&tmp) noexcept
    : List{}
{
    swap(tmp);
}

template <typename T>
List_NS::List<T>::List(std::initializer_list<T> lst)
    : List{}
{
    for (auto val : lst)
    {
        push_back(val);
    }
}

template <typename T>
void List_NS::List<T>::push_front(T value)
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

template <typename T>
void List_NS::List<T>::push_back(T value)
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

template <typename T>
bool List_NS::List<T>::empty() const noexcept
{
    return head.get() == tail;
}

template <typename T>
T List_NS::List<T>::back() const noexcept
{
    return tail->prev->value;
}

template <typename T>
T &List_NS::List<T>::back() noexcept
{
    return tail->prev->value;
}

template <typename T>
T List_NS::List<T>::front() const noexcept
{
    return head->value;
}

template <typename T>
T &List_NS::List<T>::front() noexcept
{
    return head->value;
}

template <typename T>
T &List_NS::List<T>::at(int idx)
{
    return const_cast<T &>(static_cast<List const &>(*this).at(idx));
}

template <typename T>
T const &List_NS::List<T>::at(int idx) const
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

template <typename T>
int List_NS::List<T>::size() const noexcept
{
    return sz;
}

template <typename T>
void List_NS::List<T>::swap(List &other) noexcept
{
    using std::swap;
    swap(head, other.head);
    swap(tail, other.tail);
    swap(sz, other.sz);
}

template <typename T>
List_NS::List<T> &List_NS::List<T>::operator=(List const &rhs) &
{
    List{rhs}.swap(*this);
    return *this;
}

template <typename T>
List_NS::List<T> &List_NS::List<T>::operator=(List &&rhs) & noexcept
{
    swap(rhs);
    return *this;
}

// iterator

template <typename T>
List_NS::List<T>::List_Iterator::List_Iterator(Node *ptr)
    : curr(ptr) {}

template <typename T>
typename List_NS::List<T>::List_Iterator::reference List_NS::List<T>::List_Iterator::operator*() const
{
    return curr->value;
}

template <typename T>
typename List_NS::List<T>::List_Iterator &List_NS::List<T>::List_Iterator::operator++()
{
    if (curr->next != nullptr)
    {
        curr = curr->next.get();
        return *this;
    }
}

template <typename T>
typename List_NS::List<T>::List_Iterator List_NS::List<T>::List_Iterator::operator++(int)
{
    List_Iterator tmp(*this);
    if (curr->next != nullptr)
    {
        curr = curr->next.get();
        return tmp;
    }
}

template <typename T>
typename List_NS::List<T>::List_Iterator &List_NS::List<T>::List_Iterator::operator--()
{
    if (curr->prev != nullptr)
    {
        curr = curr->prev;
        return *this;
    }
}

template <typename T>
typename List_NS::List<T>::List_Iterator List_NS::List<T>::List_Iterator::operator--(int)
{
    List_Iterator tmp(*this);
    if (curr->prev != nullptr)
    {
        curr = curr->prev;
        return tmp;
    }
}

template <typename T>
bool List_NS::List<T>::List_Iterator::operator==(const List_Iterator &rhs) const
{
    return curr == rhs.curr;
}

template <typename T>
bool List_NS::List<T>::List_Iterator::operator!=(const List_Iterator &rhs) const
{
    return curr != rhs.curr;
}

template <typename T>
typename List_NS::List<T>::List_Iterator List_NS::List<T>::begin() const
{
    return List_Iterator(head.get());
}

template <typename T>
typename List_NS::List<T>::List_Iterator List_NS::List<T>::end() const
{
    return List_Iterator(tail);
}