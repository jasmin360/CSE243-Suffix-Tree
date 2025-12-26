#pragma once
#include <string>

namespace DNA {

    int* SearchPattern(
        const std::string& seqA,
        const std::string& seqB,
        const std::string& pattern,
        int& count
    );

}
