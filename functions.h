#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>

namespace DNA {
    int* SearchPattern(const std::string&, const std::string&, int&);
    std::string* findUniqueRegion(const std::string&, int);
    std::string findMaxRepetition(const std::string&, int, int&, std::string&);
    std::string findLargestCommonRegion(const std::string&, const std::string&);
}

#endif
