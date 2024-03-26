#ifndef LIST_H
#define LIST_H

#include <initializer_list>
#include <memory>
class List
{
    // flyttade upp private då kompilator går uppifrån och ner, node behöver alltså deklareras innan min iterator då den annars inte kommer känna igen node
private:
    struct Node;
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

    class List_iterator
    {
    public:
        friend class List;
        List_iterator(const List_iterator &) = default;
        List_iterator &operator=(List_iterator const &);
        List_iterator &operator++();
        List_iterator &operator--();
        bool operator==(List_iterator const &it) const;
        bool operator!=(List_iterator const &it) const;
        int &operator*() const;
        explicit List_iterator(Node *p = nullptr) : pos(p) {}

    private:
        Node *pos;
    };
    List_iterator begin() const;
    List_iterator end() const;
    List_iterator insert(List_iterator position, const int &N);
    List sub(std::initializer_list<int> indices);
};

#endif // LIST_H
