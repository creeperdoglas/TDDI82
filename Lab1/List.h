#ifndef LIST_H
#define LIST_H

#include <initializer_list>
#include <memory>
class List
{
    // flyttade upp private då kompilator går uppifrån och ner, node behöver alltså deklareras innan min iterator då den annars inte kommer känna igen node, möjligtvis att endast kompilator fel?
    // också för att få unique ptr för head måste node deklareras innan head, går ej med en forward deklaration, så flyttade den.
private:
    struct Node
    {
        Node() = default;
        Node(int v, Node *p, std::unique_ptr<Node> n)
            : value{v}, prev{p}, next{std::move(n)} {}
        int value{};

        // prev ej nödvändig för unique_ptr
        // eftersom prev endast är för att hålla koll på föregående nod
        Node *prev{};
        std::unique_ptr<Node> next{};
    };
    std::unique_ptr<Node> head;
    Node *tail{};
    int sz{};

public:
    List();
    List(List const &);
    List(List &&) noexcept;
    List(std::initializer_list<int>);

    List &operator=(List const &) &;
    List &operator=(List &&) & noexcept;

    void push_front(int);
    void push_back(int);

    int back() const noexcept;
    int &back() noexcept;

    int front() const noexcept;
    int &front() noexcept;

    int &at(int idx);
    int const &at(int idx) const;

    int size() const noexcept;
    bool empty() const noexcept;

    void swap(List &other) noexcept;

    // från tidigare labb (gjorde en iterator till den även fast det ej var ett krav) lagt till o ändrat sånt som behövs för att iteratorn passar till den här labben o kraven
    //     class List_iterator
    //     {
    //     public:
    //         using iterator_category = std::bidirectional_iterator_tag;
    //         using value_type = int;
    //         using difference_type = std::ptrdiff_t;
    //         using pointer = value_type *;
    //         using reference = value_type &;

    //         friend class List;
    //         List_iterator(const List_iterator &) = default;
    //         List_iterator &operator=(List_iterator const &);
    //         List_iterator &operator++();
    //         List_iterator operator++(int);

    //         List_iterator &operator--();
    //         List_iterator operator--(int);

    //         bool operator==(List_iterator const &it) const;
    //         bool operator!=(List_iterator const &it) const;

    //         int &operator*() const;
    //         explicit List_iterator(Node *p = nullptr) : pos(p) {}

    //     private:
    //         Node *pos;
    //     };
    //     List_iterator begin() const;
    //     List_iterator end() const;
    //     List_iterator insert(List_iterator position, const int &N);
};

#endif
