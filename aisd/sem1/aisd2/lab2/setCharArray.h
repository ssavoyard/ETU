#ifndef SET_H
#define SET_H

#include <iostream>
#include <cstring>
using namespace std;

class Set {
private:
    char A[17];    // Array to store elements (null-terminated string)
    int n;         // Number of elements in the set

public:
    Set();                                  // Default constructor
    Set(const char*);                       // Constructor with initialization
    Set operator | (const Set&) const;      // Union operator
    Set operator & (const Set&) const;      // Intersection operator
    Set operator ~ () const;                // Complement operator
    void show() const;                      // Display the set
    bool contains(char) const;              // Check if element exists in the set
    int power() { return n; }               // Return number of elements
    void randomFill();                      // Randomly fill the set
};

// Default constructor
Set::Set() : n(0) {
    A[0] = '\0';
}

// Constructor with initialization (populate the set)
Set::Set(const char* elems) {
    n = strlen(elems);
    strcpy_s(A, elems);
}

// Union operator implementation
Set Set::operator | (const Set& B) const {
    Set result;
    strcpy_s(result.A, A);
    result.n = n;

    // Add elements from set B that are not in A
    for (int i = 0; i < B.n; ++i) {
        if (!contains(B.A[i])) {
            result.A[result.n++] = B.A[i];
        }
    }
    result.A[result.n] = '\0'; // Terminate string
    return result;
}

// Intersection operator implementation
Set Set::operator & (const Set& B) const {
    Set result;
    for (int i = 0; i < n; ++i) {
        if (B.contains(A[i])) {
            result.A[result.n++] = A[i];
        }
    }
    result.A[result.n] = '\0';
    return result;
}

// Complement operator implementation (relative to universal set)
Set Set::operator ~ () const {
    Set result;
    const char universe[] = "0123456789ABCDEF";
    for (int i = 0; i < 16; ++i) {
        if (!contains(universe[i])) {
            result.A[result.n++] = universe[i];
        }
    }
    result.A[result.n] = '\0';
    return result;
}

// Check if element exists in the set
bool Set::contains(char elem) const {
    for (int i = 0; i < n; ++i) {
        if (A[i] == elem) return true;
    }
    return false;
}

// Display the set
void Set::show() const {
    cout << "{ ";
    for (int i = 0; i < n; ++i) {
        cout << A[i] << ' ';
    }
    cout << "}" << endl;
}

// Fill the set with random elements
void Set::randomFill() {
    const char universe[] = "0123456789ABCDEF";
    n = 0;
    int numElements = rand() % 16 + 1;  // Random number of elements (1 to 16)

    while (n < numElements) {
        char randomChar = universe[rand() % 16];  // Select random character from universe
        if (!contains(randomChar)) {
            A[n++] = randomChar;  // Add character if not already in the set
        }
    }
    A[n] = '\0';  // Terminate string
}

#endif
