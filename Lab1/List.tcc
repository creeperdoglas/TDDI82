#include "List.h"
#include <iterator>
#include <utility>
#include <stdexcept>
#include <memory>
// har även testat att köra programmet med drmemory och inga minnesläckor hittades

//  i exempelvis den under så är template <typename T>  en template-deklaration där T reprsenterar en parameter och är en "placeholder" , gör så att T kan vara olika datattyper
//  i functionsdeklarationen speciferas det att det tillhör List klassen inom List_NS namnrymden (se testfallen för att se varför) och funktionen är mallad(?) med en datatyp T"

template <typename T>
struct List_NS::List<T>::Node
{
    // T value;
    // std::unique_ptr<Node> next;
    // Node *prev;

    // Node(T value = T{}, Node *prev = nullptr, std::unique_ptr<Node> next = nullptr)
    //     : value{value}, next{std::move(next)}, prev{prev}
    // {
    // }
    Node() = default;
    Node(T v, Node *p, Node *n)
        : value{v}, prev{p}, next{n} {}
    T value{};
    Node *prev{};
    std::unique_ptr<Node> next{};
};

template <typename T>
List_NS::List<T>::List()
    : head{std::make_unique<Node>()},
      tail{head.get()}, sz(0)
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

    head = std::make_unique<Node>(value, nullptr, head.release());

    head.get()->next.get()->prev = head.get();

    if (sz == 0)
    {
        tail->prev = head.get();
    }

    ++sz;
}

template <typename T>
void List_NS::List<T>::push_back(T value)
{

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
// Konstruktör
// Initierar iteratorn så att den pekar på den angivna noden.

template <typename T>
List_NS::List<T>::List_Iterator::List_Iterator(Node *ptr)
    : curr(ptr)
{
}

// Dereferensoperator *
// Returnerar en referens till värdet som noden pekar på.
template <typename T>
typename List_NS::List<T>::List_Iterator::reference List_NS::List<T>::List_Iterator::operator*() const
{
    return curr->value;
}

// Prefix Increment Operator ++
// Flyttar iteratorn till nästa nod i listan och returnerar en referens till iteratorn.
template <typename T>
typename List_NS::List<T>::List_Iterator &List_NS::List<T>::List_Iterator::operator++()
{
    if (curr->next != nullptr)
    {
        curr = curr->next.get();
    }
    return *this;
}

// Postfix Increment Operator ++
// Skapar en temporär kopia av iteratorn, flyttar sedan iteratorn till nästa nod, och returnerar kopian.
template <typename T>
typename List_NS::List<T>::List_Iterator List_NS::List<T>::List_Iterator::operator++(int)
{
    List_Iterator tmp(*this);
    if (curr->next != nullptr)
    {
        curr = curr->next.get();
    }
    return tmp;
}

// prefix decrement operator
// Flyttar iteratorn till den föregående noden i listan och returnerar en referens till iteratorn.
template <typename T>
typename List_NS::List<T>::List_Iterator &List_NS::List<T>::List_Iterator::operator--()
{
    if (curr->prev != nullptr)
    {
        curr = curr->prev;
    }
    return *this;
}

// postfix decrement operator
// Skapar en temporär kopia av iteratorn, flyttar sedan iteratorn till den föregående noden, och returnerar kopian
template <typename T>
typename List_NS::List<T>::List_Iterator List_NS::List<T>::List_Iterator::operator--(int)
{
    List_Iterator tmp(*this);
    if (curr->prev != nullptr)
    {
        curr = curr->prev;
    }
    return tmp;
}
// equality operator
// kollar  om de pekar på samma nod
template <typename T>
bool List_NS::List<T>::List_Iterator::operator==(const List_Iterator &rhs) const
{
    return curr == rhs.curr;
}

// inequality operator
// kollar om de inte pekar på samma nod
template <typename T>
bool List_NS::List<T>::List_Iterator::operator!=(const List_Iterator &rhs) const
{
    return curr != rhs.curr;
}

// begin
// returnerar en iterator som pekar på första noden i listan
template <typename T>
typename List_NS::List<T>::List_Iterator List_NS::List<T>::begin() const
{
    return List_Iterator(head.get());
}

// end
// returnerar en iterator som pekar på noden efter sista noden i listan
template <typename T>
typename List_NS::List<T>::List_Iterator List_NS::List<T>::end() const
{
    return List_Iterator(tail);
}

template <typename T>
typename List_NS::List<T>::List_Iterator::pointer List_NS::List<T>::List_Iterator::operator->() const
{
    return &(operator*());
}