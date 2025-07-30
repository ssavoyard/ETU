#ifndef SET_MACHINE_WORD_H
#define SET_MACHINE_WORD_H

#include <iostream>
#include <bitset>
#include <cstring>

using namespace std;

class Set {
private:
    unsigned short word; // Machine word (16 bits, represents set elements)

public:
    Set();                         // Default constructor
    Set(unsigned short w);         // Constructor with given word
    void randomFill();             // Fill set with random elements
    Set operator | (const Set&) const;  // Union operator
    Set operator & (const Set&) const;  // Intersection operator
    Set operator ~ () const;            // Complement operator
    void show() const;                  // Display the set
    int power() const;                  // Set power (number of elements)
};

// Default constructor initializing with an empty set
Set::Set() : word(0) {}

// Constructor with a given word value
Set::Set(unsigned short w) : word(w) {}

// Fill set with random elements
void Set::randomFill() {
    word = 0;
    int numElements = rand() % 16 + 1;  // Random number of elements (1 to 16)
    for (int i = 0; i < numElements; ++i) {
        word |= (1 << (rand() % 16));   // Set a random bit
    }
}

// Union operation
Set Set::operator | (const Set& B) const {
    return Set(word | B.word);
}

// Intersection operation
Set Set::operator & (const Set& B) const {
    return Set(word & B.word);
}

// Complement operation (16-bit complement)
Set Set::operator ~ () const {
    return Set(~word & 0xFFFF);  // Invert bits and mask to 16 bits
}

// Display the set
void Set::show() const {
    std::cout << "{ ";
    for (int i = 0; i < 16; ++i) {
        if (word & (1 << i)) {  // Check if bit is set
            if (i < 10) std::cout << char('0' + i) << ' ';
            else std::cout << char('A' + (i - 10)) << ' ';
        }
    }
    std::cout << "}" << std::endl;
}

// Set power (number of set elements)
int Set::power() const {
    return std::bitset<16>(word).count(); // Count the number of set bits
}

#endif