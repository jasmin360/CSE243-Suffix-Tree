#pragma once
#include <string>

namespace DNA {

    int* SearchPattern(
        const std::string& sequence,
        const std::string& pattern,
        int& count
    );
    std::string* findUniqueRegion(const std::string& sequence,int x);

    std::string findLargestCommonRegion(const std::string& s1,const std::string& s2);


}
