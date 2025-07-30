#ifndef SET_MASSIVE_BITS_H
#define SET_MASSIVE_BITS_H

#include <iostream>
#include <cstdlib>

class Set {
private:
    bool bits[16];                // Bit array to represent elements of the set

public:
    Set();                        // Default constructor (empty set)
    void randomFill();            // Fill set with random elements
    void add(char);               // Add element to the set
    bool contains(char) const;    // Check if an element is in the set
    Set operator | (const Set&) const;  // Union operator
    Set operator & (const Set&) const;  // Intersection operator
    Set operator ~ () const;           // Complement operator
    int power() const;                 // Set power (number of elements)
    void show() const;                 // Display the set
};

// Default constructor with empty set initialization
Set::Set() {
    for (int i = 0; i < 16; ++i) {
        bits[i] = false;
    }
}

// Fill set with random elements
void Set::randomFill() {
    for (int i = 0; i < 16; ++i) {
        bits[i] = false;
    }
    int numElements = rand() % 16 + 1;  // Random number of elements (1 to 16)
    while (power() < numElements) {
        int index = rand() % 16;
        bits[index] = true;  // Set random element
    }
}

// Add an element to the set
void Set::add(char elem) {
    int index = (elem <= '9') ? elem - '0' : elem - 'A' + 10;
    bits[index] = true;  // Set bit for this element
}

// Check if an element is in the set
bool Set::contains(char elem) const {
    int index = (elem <= '9') ? elem - '0' : elem - 'A' + 10;
    return bits[index];
}

// Union operation
Set Set::operator | (const Set& B) const {
    Set result;
    for (int i = 0; i < 16; ++i) {
        result.bits[i] = bits[i] | B.bits[i];  // Bitwise OR operation
    }
    return result;
}

// Intersection operation
Set Set::operator & (const Set& B) const {
    Set result;
    for (int i = 0; i < 16; ++i) {
        result.bits[i] = bits[i] & B.bits[i];  // Bitwise AND operation
    }
    return result;
}

// Complement operation (universal set)
Set Set::operator ~ () const {
    Set result;
    for (int i = 0; i < 16; ++i) {
        result.bits[i] = !bits[i];  // Invert bits
    }
    return result;
}

// Set power (number of elements)
int Set::power() const {
    int count = 0;
    for (int i = 0; i < 16; ++i) {
        if (bits[i]) count++;
    }
    return count;
}

// Display the set
void Set::show() const {
    std::cout << "{ ";
    for (int i = 0; i < 16; ++i) {
        if (bits[i]) {
            if (i < 10) std::cout << char('0' + i) << ' ';
            else std::cout << char('A' + (i - 10)) << ' ';
        }
    }
    std::cout << "}" << std::endl;
}

#endif