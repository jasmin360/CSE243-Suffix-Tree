#include "functions.h"
#include "Suffix.h"

namespace DNA {

int* SearchPattern(const std::string& sequence,
                   const std::string& pattern,
                   int& count)
{
    count = 0;
    Suffix tree;
    tree.makeSuffixTree(sequence + "$");
    return tree.searchPattern(pattern, count);
}

std::string* findUniqueRegion(const std::string& sequence, int x)
{
    std::string* arr = new std::string[3];
    Suffix tree;
    tree.makeSuffixTree(sequence + "$");
    tree.countLeaves(tree.root);

    std::string path = "";
    int index = 0;
    tree.findUniqueRegion(tree.root, x, 0, arr, index, path);
    return arr;
}

std::string findMaxRepetition(const std::string& sequence,
                              int x,
                              int& count,
                              std::string& res)
{
    Suffix tree;
    tree.makeSuffixTree(sequence + "$");
    tree.countLeaves(tree.root);

    int maxCount = 0;
    std::string path = "";
    tree.findMaxRepetition(tree.root, x, count, maxCount, path, res);
    return res;
}

std::string findLargestCommonRegion(const std::string& s1,
                                    const std::string& s2)
{
    Suffix tree;
    return tree.findLargestCommonRegion(s1, s2);
}

}
