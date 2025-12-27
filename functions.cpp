#include "functions.h"
#include "Suffix.h"

namespace DNA {

    int* SearchPattern(
        const std::string& seqA,
        const std::string& seqB,
        const std::string& pattern,
        int& count
    ) {
        count = 0;

        Suffix tree;
        std::string combined = seqA + "$" + seqB + "#";
        tree.makeSuffixTree(combined);

        // Call suffix tree search
        return tree.searchPattern(pattern, count);
    }
    

}
