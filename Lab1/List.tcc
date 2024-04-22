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
    Node() = default;
    Node(T const &v, Node *p, std::unique_ptr<Node> n)
        : value{v}, prev{p}, next{std::move(n)} {}
    T value{};
    std::unique_ptr<Node> next{};
    Node *prev{};
};

template <typename T>
List_NS::List<T>::List()
    : head{std::make_unique<Node>()}, tail{}, sz{}
{
    head->next = std::make_unique<Node>(T{}, head.get(), nullptr);
    tail = head->next.get();
}

template <typename T>
List_NS::List<T>::List(List_NS::List<T> const &other)
    : List{}
{
    for (Node *tmp{other.head->next.get()}; tmp != other.tail; tmp = tmp->next.get())
    {
        push_back(tmp->value);
        // tmp = tmp->next.get();
    }
}

template <typename T>
List_NS::List<T>::List(List_NS::List<T> &&tmp) noexcept
    : List{}
{
    swap(tmp);
}

template <typename T>
List_NS::List<T>::List(std::initializer_list<T> const &lst)
    : List{}
{
    for (const auto val : lst)
    {
        push_back(val);
    }
}

template <typename T>
void List_NS::List<T>::push_front(T const &value)
{
    Node *old_first{head->next.get()};
    head->next = std::make_unique<Node>(value, head.get(), std::move(head->next));
    old_first->prev = head->next.get();

    ++sz;
}

template <typename T>
void List_NS::List<T>::push_back(T const &value)
{
    Node *old_last{this->tail->prev};
    old_last->next = std::make_unique<Node>(value, old_last, std::move(old_last->next));
    tail->prev = old_last->next.get();
    ++sz;
}

template <typename T>
bool List_NS::List<T>::empty() const noexcept
{
    // return head.get() == tail;
    return head->next.get() == tail;
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
    // return head->value
    return head->next->value;
}

template <typename T>
T &List_NS::List<T>::front() noexcept
{
    // return head->value
    return head->next->value;
}

template <typename T>
T &List_NS::List<T>::at(int idx)
{
    // return const_cast<T &>(static_cast<List const &>(*this).at(idx));
    return const_cast<T &>(static_cast<List_NS::List<T> const *>(this)->at(idx));
}

template <typename T>
T const &List_NS::List<T>::at(int idx) const
{
    if (idx >= sz)
        throw std::out_of_range{"Index not found"};
    // Node *tmp{head.get()};
    Node *tmp{head->next.get()};
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
void List_NS::List<T>::swap(List_NS::List<T> &other) noexcept
{
    using std::swap;
    swap(head, other.head);
    swap(tail, other.tail);
    swap(sz, other.sz);
}

template <typename T>
List_NS::List<T> &List_NS::List<T>::operator=(List<T> const &rhs) &
{
    List{rhs}.swap(*this);
    return *this;
}

template <typename T>
List_NS::List<T> &List_NS::List<T>::operator=(List<T> &&rhs) & noexcept
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
typename List_NS::List<T>::List_Iterator List_NS::List<T>::begin()
{
    // return List_Iterator(head.get());
    return List_Iterator((head->next).get());
}

// end
// returnerar en iterator som pekar på noden efter sista noden i listan
template <typename T>
typename List_NS::List<T>::List_Iterator List_NS::List<T>::end()
{
    return List_Iterator(tail);
}

template <typename T>
typename List_NS::List<T>::List_Iterator::pointer List_NS::List<T>::List_Iterator::operator->() const
{
    // return &(operator*());
    return &(curr->value);
}