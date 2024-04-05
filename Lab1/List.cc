#include "List.h"
#include <iterator>
#include <utility>
#include <stdexcept>
#include <memory>

// svar på frågorna:
// listan startar en tom lista med en head pekare som pekar till nullptr och en tail pekare.Det betyder att i det initiala tillståndet innan några element har lagts till finns det inga noder. då inga sentinels

// När ett element läggs till i början av listan (push_front), skapas en ny nod som blir det nya huvudet (head) av listan. Den nya nodens next pekare kommer att peka på den tidigare huvudnoden. Om listan var tom innan elementet lades till, blir denna nya nod både huvud och svans (tail) av listan. detta innebär att elementet placeras precis före det första elementet som redan finns i listan.

// är ett element läggs till sist i listan (push_back), skapas en ny nod som placeras efter den nuvarande sista noden. Om listan var tom, kallas 33push_front för att lägga till den första noden istället, vilket gör den nya noden både till huvudet och svansen av listan. Om listan inte var tom, blir den nya noden den sista noden, och dess next pekare kommer att vara nullptr, medan prev pekaren länkar till den tidigare sista noden. Kort sagt, elementet placeras efter det sista elementet som redan finns i listan.

// Det finns två versioner av at-funktionen för att få tillgång till element i listan både för icke-const och const instanser av listan:
// används med icke-const instanser av listan. Detta göt att man kan modifiera det element som at-funktionen returnerar en referens till. medans den med const gör så at inte modifierar listan

// Icke-const versionen av at-funktionen hämtar ett värde från listan men också modifiera det returnerade värdet. Detta uppnås genom att returnera en referens till värdet (T&) snarare än värdet självt. Icke-const versionen använder const_cast och static_cast för att kasta bort const-qualifier från den const versionen av at-metoden. Detta gör det möjligt att återanvända logiken i den const-versionen för att hitta det önskade elementet men sedan returnera det som en icke-const referens, vilket tillåter ändringar av elementets värde.

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
    : head{std::make_unique<Node>()},
      tail{head.get()}, sz{}
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