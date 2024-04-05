#include "List.h"
#include <iterator>
#include <utility>
#include <stdexcept>
#include <memory>
// har även testat att köra programmet med drmemory och verkar som inga minnesläckor

//  i exempelvis den under så är template <typename T>  en template-deklaration där T reprsenterar en parameter och är en "placeholder" , gör så att T kan vara olika datattyper
//  i functionsdeklarationen speciferas det att det tillhör List klassen inom List_NS namnrymden (se testfallen för att se varför) och funktionen är mallad(?) med en datatyp T"

template <typename T>
List_NS::List<T>::List()
    : head{std::make_unique<Node>()},
      tail{head.get()}, sz{}
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

    // Skapar en ny nod i början av listan med det givna värdet.
    // Den nya nodens 'next' pekare sätts till den nuvarande 'head' noden,
    // och den nuvarande 'head' nodens 'prev' pekare uppdateras till den nya noden.
    head = std::make_unique<Node>(value, nullptr, head.release());

    // den nya nodens 'next' pekare (som nu är den gamla head-noden)
    // pekar tillbaka på den nya noden som nu är head.
    head.get()->next.get()->prev = head.get();

    // Om listan var tom innan insättningen, uppdatera även 'tail' pekaren
    // att peka på den nya noden eftersom listan nu endast innehåller en nod.
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

    // Kontrollerar om listan är tom.
    // Om listan är tom, använd 'push_front' för att lägga till elementet,
    // eftersom det i en tom lista inte spelar någon roll om vi lägger till
    // elementet i början eller i slutet.

    /*Node * old_last { tail->prev };
    old_last->next = std::make_unique<Node>(value, old_last, tail);
    tail->prev = old_last->next.get();*/
    if (empty())
    {
        push_front(value);
    }
    else
    {
        // För en icke-tom lista, frigör 'next' pekaren för noden innan 'tail',
        // och skapa sedan en ny nod som sätter dess 'prev' till den nuvarande sista noden
        // och dess 'next' till 'tail'. Den nuvarande sista nodens 'next' uppdateras till den nya noden.
        tail->prev->next.release();
        tail->prev->next = std::make_unique<Node>(value, tail->prev, tail);

        // Uppdaterar 'tail->prev' att peka på den nya noden,
        // vilket lägger till den nya noden i slutet av listan
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
        return *this;
    }
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
        return tmp;
    }
}

// prefix decrement operator
// Flyttar iteratorn till den föregående noden i listan och returnerar en referens till iteratorn.
template <typename T>
typename List_NS::List<T>::List_Iterator &List_NS::List<T>::List_Iterator::operator--()
{
    if (curr->prev != nullptr)
    {
        curr = curr->prev;
        return *this;
    }
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
        return tmp;
    }
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
// används som en sentinel, däremot har inte listan några sentinels
template <typename T>
typename List_NS::List<T>::List_Iterator List_NS::List<T>::end() const
{
    return List_Iterator(tail);
}