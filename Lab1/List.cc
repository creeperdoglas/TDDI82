#include "List.h"
#include <iterator>
#include <utility>
#include <stdexcept>
#include <memory>

// struct List::Node
// {
//     Node() = default;
//     Node(int v, Node *p, std::unique_ptr<Node> n)
//         : value{v}, prev{p}, next{std::move(n)} {}
//     int value{};

//     // prev ej nödvändig för unique_ptr
//     // eftersom prev endast är för att hålla koll på föregående nod
//     Node *prev{};
//     std::unique_ptr<Node> next{};
// };

List::List()
    // head äger en resurs, tail pekar på en resurs, därför bör head vara en unique_ptr
    : head{std::make_unique<Node>()}, tail{}, sz{}
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
// void List::push_back(int value)
// {
//     Node *old_last = tail->prev;
//     std::unique_ptr<Node> newNode = std::make_unique<Node>(value, old_last, nullptr);
//     Node *newNodeRaw = newNode.get();
//     old_last->next = std::move(newNode);
//     tail->prev = newNodeRaw;
//     ++sz;
// }

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
