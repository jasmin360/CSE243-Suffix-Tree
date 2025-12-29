#pragma once
#include <string>

namespace DNA {

    int* SearchPattern(
        const std::string& sequence,
        const std::string& pattern,
        int& count
    );
    std::string* findUniqueRegion(const std::string& sequence,int x);
    void findMaxRepetition(const std::string& sequence, int x, int& count, const std::string& res);
    std::string findLargestCommonRegion(const std::string& s1,const std::string& s2);

    std::string findMaxRepetition(const std::string& sequence, int x, int& count, std::string& res);


}
