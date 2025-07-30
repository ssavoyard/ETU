#ifndef SET_LINKED_LIST_H
#define SET_LINKED_LIST_H

#include <iostream>

class Set {
private:
    struct Node {                  // Node structure (list element)
        char el;                   // Element value
        Node* next;                // Pointer to the next element
        Node(char e, Node* n = nullptr) : el(e), next(n) {} // Node constructor
    };

    Node* head;                    // Head of the list (start of set)

public:
    Set();                         // Default constructor (empty set)
    ~Set();                        // Destructor (cleans up memory)
    void randomFill();             // Fill set with random elements
    void add(char);                // Add an element to the set
    bool contains(char) const;     // Check if an element is in the set
    Set operator | (const Set&) const;  // Union operator
    Set operator & (const Set&) const;  // Intersection operator
    Set operator ~ () const;            // Complement operator
    int power() const;                  // Set power (number of elements)
    void show() const;                  // Display the set
};

// Default constructor
Set::Set() : head(nullptr) {}

// Destructor, freeing memory
Set::~Set() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

// Add an element to the set
void Set::add(char elem) {
    if (!contains(elem)) {
        head = new Node(elem, head); // Add new node at the beginning of the list
    }
}

// Check if an element is in the set
bool Set::contains(char elem) const {
    Node* temp = head;
    while (temp) {
        if (temp->el == elem) return true;
        temp = temp->next;
    }
    return false;
}

// Union operation
Set Set::operator | (const Set& B) const {
    Set result;

    // Add all elements from the current set
    Node* temp = head;
    while (temp) {
        result.add(temp->el);
        temp = temp->next;
    }

    // Add elements from B that are not already in the current set
    temp = B.head;
    while (temp) {
        result.add(temp->el);
        temp = temp->next;
    }

    return result;
}

// Intersection operation
Set Set::operator & (const Set& B) const {
    Set result;
    Node* temp = head;
    while (temp) {
        if (B.contains(temp->el)) {
            result.add(temp->el);
        }
        temp = temp->next;
    }
    return result;
}

// Complement operation
Set Set::operator ~ () const {
    Set result;
    const char universe[] = "0123456789ABCDEF"; // Universe of possible elements
    for (int i = 0; i < 16; ++i) {
        if (!contains(universe[i])) {
            result.add(universe[i]);
        }
    }
    return result;
}

// Set power (number of elements)
int Set::power() const {
    int count = 0;
    Node* temp = head;
    while (temp) {
        count++;
        temp = temp->next;
    }
    return count;
}

// Display the set
void Set::show() const {
    Node* temp = head;
    std::cout << "{ ";
    while (temp) {
        std::cout << temp->el << ' ';
        temp = temp->next;
    }
    std::cout << "}" << std::endl;
}

// Fill the set with random elements
void Set::randomFill() {
    const char universe[] = "0123456789ABCDEF"; // Universe of possible elements
    int numElements = rand() % 16 + 1;  // Random number of elements (1 to 16)

    while (power() < numElements) {
        char randomChar = universe[rand() % 16];   // Select random character from universe
        add(randomChar);  // Add character if not already in the set
    }
}

#endif
