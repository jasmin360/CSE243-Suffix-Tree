#include "functions.h"
#include "Suffix.h"

namespace DNA {

    int* SearchPattern(
        const std::string& sequence,
        const std::string& pattern,
        int& count
    ) {
        count = 0;

        Suffix tree;
        std::string seq = sequence + "$";
        tree.makeSuffixTree(seq);

        // Call suffix tree search
        return tree.searchPattern(pattern, count);
    }
    
    std::string* findUniqueRegion(const std::string& sequence,int x){
        // Return a single-element array containing the found unique region (or empty string if none)
        std::string* arr = new std::string[1];
        std::string newSequence = sequence + "$";
        Suffix tree;
        tree.makeSuffixTree(newSequence);
        tree.countLeaves(tree.root);
        int temp =0;
        tree.findUniqueRegion(tree.root, 0, x, arr, temp);
        return arr;

    }
}
