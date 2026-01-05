#include "functions.h"
#include <cstring>
#include <string>

extern "C" {

/// -------- SEARCH PATTERN --------
int* dna_search_pattern(
    const char* sequence,
    const char* pattern,
    int* out_count
) {
    std::string seq(sequence);
    std::string pat(pattern);

    return DNA::SearchPattern(seq, pat, *out_count);
}

/// -------- UNIQUE REGIONS --------
/// returns array of 3 C-strings
const char** dna_unique_regions(const char* sequence, int x) {

    static const char* result[3] = { nullptr, nullptr, nullptr };

    // Clear previous memory
    for (int i = 0; i < 3; i++) {
        if (result[i]) {
            free((void*)result[i]);
            result[i] = nullptr;
        }
    }

    // Call your existing C++ function
    std::string seq(sequence);
    std::string* arr = DNA::findUniqueRegion(seq, x);

    // Copy results into C-strings
    for (int i = 0; i < 3; i++) {
        if (!arr[i].empty()) {
            result[i] = _strdup(arr[i].c_str()); // Windows-safe
        }
    }

    delete[] arr;  // VERY IMPORTANT

    return result;
}

/// -------- MAX REPETITION --------
const char* dna_max_repetition(
    const char* sequence,
    int x,
    int* out_count
) {
    static std::string res;
    std::string seq(sequence);

    DNA::findMaxRepetition(seq, x, *out_count, res);
    return res.c_str();
}

/// -------- LARGEST COMMON REGION --------
const char* dna_largest_common(
    const char* s1,
    const char* s2
) {
    static std::string res;
    res = DNA::findLargestCommonRegion(s1, s2);
    return res.c_str();
}

}
