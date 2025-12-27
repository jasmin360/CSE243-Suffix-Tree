#include "functions.h"
#include "Suffix.h"
#include "Suffix.cpp"

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
    
    string* findUniqueRegion(const string& sequence,int x){
        string* arr = new string[3];
        string newSequence= sequence + "$";
        Suffix tree;
        tree.makeSuffixTree(newSequence);
        tree.countLeaves(tree.root);
        tree.findUniqueRegion(tree.root, x, 0, arr, 0);

    }
}
