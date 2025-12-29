#include "functions.h"
#include "Suffix.h"

namespace DNA {

    int* DNA :: SearchPattern(
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
    
    std::string* DNA :: findUniqueRegion(const std::string& sequence,int x){
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

    std::string DNA :: findLargestCommonRegion(const std::string& s1,const std::string& s2) {
        Suffix tree;
        return tree.findLargestCommonRegion(s1, s2);
    }

    std::string DNA :: findMaxRepetition(const std::string& sequence, int x) {
        Suffix tree;
        std::string seq = sequence + "$";
        tree.makeSuffixTree(seq);
        tree.countLeaves(tree.root);
        std::string res = "";
        int count = 0;
        tree.findMaxRepetition(tree.root, x, count, 0, res);
        return res;
	}
}
