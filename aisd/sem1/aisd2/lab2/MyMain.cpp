#include "setCharArray.h"
//#include "setLinkedList.h"
//#include "setLinkedListNoOverload.h"
//#include "setLinkedListTracking.h" // Version with function call tracking, set q0 = 1
//#include "setMachineWord.h"
//#include "setMassiveBits.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

const long q0 = 100000;

int main() {
    srand(time(0));  // Initialize random number generator with current time
    
    int powerE;      // Variable to store the power (number of elements) of set E
    Set A, B, C, D;  // Initialize sets A, B, C, D

    // Uncomment to define sets (char array) 
    // Set A ("1259ABC");
    // Set B ("15ACEF");
    // Set C ("35AF");
    // Set D ("2468F");

    // Linked list & massive of bites
    // A.add('1'); A.add('2'); A.add('5'); A.add('9'); A.add('A'); A.add('B'); A.add('C');
    // B.add('1'); B.add('5'); B.add('A'); B.add('C'); B.add('E'); B.add('F');
    // C.add('3'); C.add('5'); C.add('A'); C.add('F');
    // D.add('2'); D.add('4'); D.add('6'); D.add('8'); D.add('F');

    // Machine word
    // A = Set((1 << 1) | (1 << 2) | (1 << 5) | (1 << 9) | (1 << 10) | (1 << 11) | (1 << 12));
    // B = Set((1 << 1) | (1 << 5) | (1 << 10) | (1 << 12) | (1 << 14) | (1 << 15));
    // C = Set((1 << 3) | (1 << 5) | (1 << 10) | (1 << 15));
    // D = Set((1 << 2) | (1 << 4) | (1 << 6) | (1 << 8) | (1 << 15));

    // Randomly fill sets A, B, C, and D with elements (comment if defining manually)
    A.randomFill();
    B.randomFill();
    C.randomFill();
    D.randomFill();

    // Display the elements of sets A, B, C, and D
    cout << "A: "; A.show();
    cout << "B: "; B.show();
    cout << "C: "; C.show();
    cout << "D: "; D.show();

    // Performance measurement and set operations testing
    clock_t begin = clock(); // Start measuring time

    for (long q = 0; q < q0; q++) {
        // Inion of sets A and C
        Set AC = A | C;

        // Intersection of AC with the complements of B and D
        Set E = (AC & ~B) & ~D;

        // Display the result of the first iteration
        if (q == 0) {
            cout << "E (result): "; E.show();
            powerE = E.power(); // Calculate and store the power of E
        }
    }

    clock_t end = clock(); // Stop measuring time

    // Average power of the sets
    cout << "Middle power = " <<
        (A.power() + B.power() + C.power() + D.power() + powerE) / 5 <<
        " \nTime = " << double(end - begin)/CLOCKS_PER_SEC << " / " << q0 << endl;

    cin.get(); // Wait for the user to press Enter before exiting
    return 0;
}
