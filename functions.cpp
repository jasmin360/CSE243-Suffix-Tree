#include <iostream>
#include "functions.h"
#include "Suffix.h"
using namespace std;

namespace DNA{
    

    bool SearchPattern(const string& seqA, const string& seqB, const string& pattern){
        Suffix tree;
        tree.makeSuffixTree(seqA + "$" + seqB + "#");
        // Implement search logic here
    }
    //2nd function
    //3rd function
    //... other functions
}