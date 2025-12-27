#pragma once
#include <string>

namespace DNA {

    int* SearchPattern(
        const std::string& sequence,
        const std::string& pattern,
        int& count
    );
    string* findUniqueRegion(const string& sequence,int x);

}
