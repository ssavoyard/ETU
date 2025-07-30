#ifndef SET_LINKED_LIST_H
#define SET_LINKED_LIST_H

#include <iostream>

class Set {
private:
    struct Node {
        char el;                   // Element of the set
        Node* next;                // Pointer to the next node
        Node(char e, Node* n = nullptr) : el(e), next(n) {} // Constructor for Node

        // Overload new and delete operators
        void* operator new(size_t size) {
            return ::operator new(size);
        }

        void operator delete(void* ptr) {
            ::operator delete(ptr);
        }
    };

    Node* head;                    // Pointer to the head node (start of the set)
    static int tag_counter;         // Counter to generate unique tags for sets
    int tag;                        // Unique tag for this set

public:
    Set();                         // Default constructor
    Set(const Set&);               // Copy constructor
    Set(Set&&);                    // Move constructor
    ~Set();                        // Destructor
    Set& operator=(const Set&);    // Copy assignment operator
    Set& operator=(Set&&);         // Move assignment operator

    void randomFill();             // Fill the set with random elements
    void add(char);                // Add an element to the set
    bool contains(char) const;     // Check if an element is in the set
    Set operator | (const Set&) const;  // Union of two sets
    Set operator & (const Set&) const;  // Intersection of two sets
    Set operator ~ () const;            // Complement of the set
    int power() const;                  // Size of the set (number of elements)
    void show() const;                  // Print the set

    void printTag() const;         // Show the tag of the set
};

// Initialize static tag counter
int Set::tag_counter = 0;

// Default constructor
Set::Set() : head(nullptr), tag(++tag_counter) {}

// Copy constructor
Set::Set(const Set& other) : head(nullptr), tag(++tag_counter) {
    Node* temp = other.head;
    while (temp) {
        add(temp->el);
        temp = temp->next;
    }
}

// Move constructor
Set::Set(Set&& other) : head(other.head), tag(++tag_counter) {
    other.head = nullptr;
}

// Destructor
Set::~Set() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

// Copy assignment operator
Set& Set::operator=(const Set& other) {
    if (this != &other) {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }

        head = nullptr;
        Node* temp = other.head;
        while (temp) {
            add(temp->el);
            temp = temp->next;
        }
    }
    return *this;
}

// Move assignment operator
Set& Set::operator=(Set&& other) {
    if (this != &other) {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }

        head = other.head;
        other.head = nullptr;
    }
    return *this;
}

// Add an element to the set
void Set::add(char elem) {
    if (!contains(elem)) {
        head = new Node(elem, head); // Insert the new node at the beginning
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

// Union of two sets
Set Set::operator | (const Set& B) const {
    Set result;

    // Add all elements from the first set
    Node* temp = head;
    while (temp) {
        result.add(temp->el);
        temp = temp->next;
    }

    // Add elements from the second set that are not already in the first set
    temp = B.head;
    while (temp) {
        result.add(temp->el);
        temp = temp->next;
    }

    return result;
}

// Intersection of two sets
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

// Complement of the set
Set Set::operator ~ () const {
    Set result;
    const char universe[] = "0123456789ABCDEF"; // Universal set
    for (int i = 0; i < 16; ++i) {
        if (!contains(universe[i])) {
            result.add(universe[i]);
        }
    }
    return result;
}

// Size of the set
int Set::power() const {
    int count = 0;
    Node* temp = head;
    while (temp) {
        count++;
        temp = temp->next;
    }
    return count;
}

// Print the set
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
    const char universe[] = "0123456789ABCDEF"; // Universal set
    int numElements = rand() % 16 + 1;  // Number of elements (1 to 16)

    while (power() < numElements) {
        char randomChar = universe[rand() % 16];   // Randomly pick a character
        add(randomChar);  // Add the character if it's not already in the set
    }
}

// Print the tag of the set
void Set::printTag() const {
    std::cout << "Set " << tag << std::endl;
}

#endif // SET_LINKED_LIST_H