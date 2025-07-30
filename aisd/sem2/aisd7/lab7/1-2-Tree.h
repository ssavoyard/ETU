// //1-2-Tree.h
// #ifndef TREE_1_2_H
// #define TREE_1_2_H

// #include <iostream>
// #include <string>
// #include <stack>
// #include <utility>
// #include <algorithm>
// #include <vector>
// #include <random>

// // Node structure for 1-2 Tree
// struct Node {
//     int key;
//     Node *left, *right;
//     bool next = false; // true if node contains 2 keys
    
//     Node(int k, Node *l = nullptr, Node *r = nullptr) 
//         : key(k), left(l), right(r) {}
//     ~Node() { delete left; delete right; }
    
//     void Display(int level, int col, int& max_level) const;
//     void Out(int row, int col) const;
// };

// using MyStack = std::stack<std::pair<Node*, int>>;

// // Iterator for Tree class
// struct myiter {
//     using iterator_category = std::forward_iterator_tag;
//     using value_type = int;
//     using difference_type = std::ptrdiff_t;
//     using pointer = int*;
//     using reference = int&;
    
//     myiter(Node *p = nullptr) : Ptr(p) {}
//     myiter(Node *p, MyStack St) : Ptr(p), St(St) {}
    
//     bool operator==(const myiter &Other) const { return Ptr == Other.Ptr; }
//     bool operator!=(const myiter &Other) const { return !(*this == Other); }
//     myiter& operator++();
//     myiter operator++(int) { myiter tmp(*this); ++*this; return tmp; }
//     reference operator*() const { return Ptr->key; }
//     pointer operator->() const { return &Ptr->key; }
    
//     Node *Ptr;
//     MyStack St;
// };

// // 1-2 Tree class implementation
// class Tree {
//     Node *root = nullptr;
//     int count = 0;
//     int tree_height = 0;
    
// public:
//     using value_type = int;
    
//     Tree() = default;
//     ~Tree() { delete root; }
    
//     std::pair<myiter, bool> insert(int k, myiter where = myiter());
//     bool erase(int k);
//     void clear() { delete root; root = nullptr; count = 0; tree_height = 0; }
//     int size() const { return count; }
//     int height() const { return tree_height; }
    
//     myiter begin() const;
//     myiter end() const { return myiter(); }
    
//     void Display() const;
    
//     template<typename Iter>
//     Tree(Iter first, Iter last) : Tree() { 
//         for (; first != last; ++first) insert(*first); 
//     }
// };

// // Output iterator adapter for set operations
// template <typename Container>
// class outiter {
//     Container& container;
//     typename Container::value_type value;
//     myiter iter;
// public:
//     using iterator_category = std::output_iterator_tag;
    
//     explicit outiter(Container& c, myiter it) : container(c), iter(it) {}
    
//     outiter& operator*() { return *this; }
//     outiter& operator++() { return *this; }
//     outiter& operator++(int) { return *this; }
    
//     outiter& operator=(const typename Container::value_type& val) {
//         value = val;
//         iter = container.insert(value, iter).first;
//         return *this;
//     }
// };

// template <typename Container>
// inline outiter<Container> outinserter(Container& c, myiter it) {
//     return outiter<Container>(c, it);
// }

// int random_int(int min, int max) {
//     static std::random_device rd;
//     static std::mt19937 gen(rd());
//     std::uniform_int_distribution<> distrib(min, max);
//     return distrib(gen);
// }

// // Function declarations for set operations
// void execute_set_operations();
// template<typename Iter>
// void printSet(const std::string& name, Iter begin, Iter end);

// #endif

// 1-2-Tree.h
#ifndef TREE_1_2_H
#define TREE_1_2_H

#include <iostream>
#include <string>
#include <stack>
#include <utility>
#include <algorithm>
#include <vector>
#include <random>

// Node structure for 1-2 Tree
struct Node {
    int key;
    int count = 1;  // Number of duplicates
    Node *left, *right;
    bool next = false;
    
    Node(int k, Node *l = nullptr, Node *r = nullptr) 
        : key(k), left(l), right(r) {}
    ~Node() { delete left; delete right; }
    
    void Display(int level, int col, int& max_level) const;
    void Out(int row, int col) const;
};

using MyStack = std::stack<std::pair<Node*, int>>;

// Iterator for Tree class
struct myiter {
    using iterator_category = std::forward_iterator_tag;
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = int*;
    using reference = int&;
    
    myiter(Node *p = nullptr) : Ptr(p), current_count(0) {
        if (p) current_count = 1;
    }
    myiter(Node *p, MyStack St) : Ptr(p), St(St), current_count(0) {
        if (p) current_count = 1;
    }
    
    bool operator==(const myiter &Other) const { return Ptr == Other.Ptr; }
    bool operator!=(const myiter &Other) const { return !(*this == Other); }
    myiter& operator++();
    myiter operator++(int) { myiter tmp(*this); ++*this; return tmp; }
    reference operator*() const { return Ptr->key; }
    pointer operator->() const { return &Ptr->key; }
    
    Node *Ptr;
    MyStack St;
    int current_count = 1;
};

// 1-2 Tree class implementation
class Tree {
    Node *root = nullptr;
    int count = 0;
    int tree_height = 0;
    bool is_set = true; // Flag to distinguish between set and sequence
    
public:
    using value_type = int;
    
    Tree(bool is_set = true) : is_set(is_set) {}
    ~Tree() { delete root; }
    
    std::pair<myiter, bool> insert(int k, myiter where = myiter());
    bool erase(int k);
    bool erase_all(int k);
    void clear() { delete root; root = nullptr; count = 0; tree_height = 0; }
    int size() const { return count; }
    int height() const { return tree_height; }
    
    myiter begin() const;
    myiter end() const { return myiter(); }
    
    void Display() const;
    
    template<typename Iter>
    Tree(Iter first, Iter last, bool is_set = true) : is_set(is_set) { 
        for (; first != last; ++first) insert(*first); 
    }
    
    void set_mode(bool set_mode) { is_set = set_mode; }
};

// Output iterator adapter for set operations
template <typename Container>
class outiter {
    Container& container;
    typename Container::value_type value;
    myiter iter;
public:
    using iterator_category = std::output_iterator_tag;
    
    explicit outiter(Container& c, myiter it) : container(c), iter(it) {}
    
    outiter& operator*() { return *this; }
    outiter& operator++() { return *this; }
    outiter& operator++(int) { return *this; }
    
    outiter& operator=(const typename Container::value_type& val) {
        value = val;
        iter = container.insert(value, iter).first;
        return *this;
    }
};

template <typename Container>
inline outiter<Container> outinserter(Container& c, myiter it) {
    return outiter<Container>(c, it);
}

int random_int(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// Function declarations
void execute_set_operations();
void execute_sequence_operations();
template<typename Iter>
void printSet(const std::string& name, Iter begin, Iter end);

#endif
