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
        std::string* arr = new std::string[1];
        std::string newSequence = sequence + "$";
        Suffix tree;
        tree.makeSuffixTree(newSequence);
        tree.countLeaves(tree.root);
        int temp =0;
        string path = "";
        int index = 0;
        tree.findUniqueRegion(tree.root, x, 0, arr, index, path); 
        return arr;

    }

    std::string DNA :: findMaxRepetition(const std::string& sequence, int x, int& count, string& res){
        std::string newSequence = sequence + "$";
        Suffix tree;
        tree.makeSuffixTree(newSequence);
        tree.countLeaves(tree.root);
        int temp =0;
        string path = "";
        int maxCount = 0;
        tree.findMaxRepetition(tree.root, 4, maxCount, 0, path, res); 
        return res;

    }
    

    std::string DNA :: findLargestCommonRegion(const std::string& s1,const std::string& s2) {
        Suffix tree;
        return tree.findLargestCommonRegion(s1, s2);
    }
}