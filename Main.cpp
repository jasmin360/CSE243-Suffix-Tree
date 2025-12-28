#include <iostream>
#include <string>
#include "functions.h"

// Helper to print arrays easily
void PrintIndices(int* indices, int count) {
    if (count == 0 || indices == nullptr) {
        std::cout << " -> Pattern not found." << std::endl;
        return;
    }
    std::cout << " -> Found at indices: ";
    for (int i = 0; i < count; i++) {
        std::cout << indices[i];
        if (i < count - 1) std::cout << ", ";
    }
    std::cout << std::endl;
}

int main() {
    // 1. Setup Data
    // We create a sequence that definitely has repeats and unique parts
    // "TG" appears at 0 and 6. "ACGT" appears once.
    std::string sequence = "TGACTGTGACGT";
    std::string pattern = "TG";
    int uniqueLength = 4; // Looking for a unique string of length 4

    std::cout << "========================================" << std::endl;
    std::cout << "      DNA SUFFIX TREE CONSOLE TEST      " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Sequence: " << sequence << std::endl;
    std::cout << "Length:   " << sequence.length() << std::endl;
    std::cout << "----------------------------------------" << std::endl << std::endl;


    // --- TEST 1: SEARCH PATTERN ---
    std::cout << "[TEST 1] Searching for pattern: \"" << pattern << "\"" << std::endl;

    int count = 0;
    // CALL YOUR FUNCTION
    int* indices = DNA::SearchPattern(sequence, pattern, count);

    // Print Results
    PrintIndices(indices, count);

    // CLEANUP
    delete[] indices;
    std::cout << std::endl;


    // --- TEST 2: UNIQUE REGION ---
    std::cout << "[TEST 2] Finding unique region of length: " << uniqueLength << std::endl;

    // CALL YOUR FUNCTION
    std::string* uniqueResult = DNA::findUniqueRegion(sequence, uniqueLength);

    // Print Results
    if (uniqueResult != nullptr && !uniqueResult[0].empty()) {
        std::cout << " -> Found Unique Region: " << uniqueResult[0] << std::endl;
    }
    else {
        std::cout << " -> No unique region of that length found." << std::endl;
    }

    // CLEANUP
    delete[] uniqueResult;

    std::cout << "\n========================================" << std::endl;
    std::cout << "              TEST COMPLETE             " << std::endl;
    std::cout << "========================================" << std::endl;

    // Keep console open
    std::cin.get();
    return 0;
}