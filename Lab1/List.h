#ifndef LIST_H
#define LIST_H

#include <initializer_list>
#include <memory>
#include <iterator> //för att använda std::bidirectional_iterator_tag

// Information om komplettering:
//   Kompletteringen kan gälla hela filen och alla filer i labben,
//   så får ni komplettering på en sak, kan samma sak förekomma på
//   fler ställen utan att jag skrivit det.
//
//   Komplettering lämnas in via sendlab efter senast en (1) vecka
//
//   Har ni frågor om kompletteringen kan ni maila mig på:
//   Malte.Nilsson@liu.se

// Komplettering: Eftersom listan nu kan innehålla vilken typ som helst ska vi undvika onödiga kopieringar. Se över all er kod! (Det gäller även loopar, returtyper, etc)

// Komplettering: Flytta tillbaka implementationen av node till (t)cc-filen.
// Initiera inte variabler i h filen utan gör det i datamedlemsinitieringslistan.
// Alternativt skapa en tom destruktor för att undvika size_of problemet som antagligen gjorde att ni flyttade den till .h filen
/// fixat

// Komplettering: Flytta ägarskapet med hjälp utav std::move istället för att använda .release

// Kommentar: difference_type är typen på avståndet mellan två iteratorer, och borde därför vara int eller ptrdiff_t.
/// fixat

// Kommentar: att sätta const på t.ex. begin och end är missvisande eftersom det med en iterator går att ändra på listan.

// Noterade kommentaren vid end() "används som en sentinel, däremot har inte listan några sentinels"
// Svar: En tom lista har två noder som skapas i konstruktorn. (sentinel nodes)
/// juste

namespace List_NS
{
    template <typename T>

    class List
    {
    private:
        struct Node;

        std::unique_ptr<Node> head{};
        Node *tail{};
        int sz{};

    public:
        List();
        List(List const &);
        List(List &&) noexcept;
        List(std::initializer_list<T>);

        List &operator=(List const &) &;
        List &operator=(List &&) & noexcept;

        void push_front(T);
        void push_back(T);

        T back() const noexcept;
        T &back() noexcept;

        T front() const noexcept;
        T &front() noexcept;

        T &at(int idx);
        T const &at(int idx) const;

        int size() const noexcept;
        bool empty() const noexcept;

        void swap(List &other) noexcept;

        class List_Iterator
        {
        public:
            friend class List;
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = ptrdiff_t;
            using pointer = T *;
            using reference = T &;

            reference operator*() const;

            List_Iterator &operator++();
            List_Iterator operator++(int);

            List_Iterator &operator--();
            List_Iterator operator--(int);

            bool operator==(const List_Iterator &) const;
            bool operator!=(const List_Iterator &) const;
            pointer operator->() const;

        private:
            List_Iterator();
            List_Iterator(Node *ptr);
            Node *curr{};
        };

        List_Iterator begin() const;
        List_Iterator end() const;
    };
}

#include "List.tcc"

#endif // LIST_H