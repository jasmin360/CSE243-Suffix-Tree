#include "functions.h"
#include "Suffix.h"
#include <vector>
#include <string>

using namespace std;

namespace DNA {

std::vector<int> SearchPattern(
    const std::string& seqA,
    const std::string& seqB,
    const std::string& pattern
) {
    Suffix tree;

    // Build combined suffix tree
    std::string combined = seqA + "$" + seqB + "#";
    tree.makeSuffixTree(combined);

    // Call suffix tree search
    return tree.searchPattern(pattern);
}

}
