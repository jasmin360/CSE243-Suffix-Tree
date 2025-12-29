#include <iostream>
#include "Suffix.h"
#include <string>
using namespace std;

// --- Suffix Constructor ---
Suffix::Suffix()
{
    remaining = 0;
    root = new Node(0, nullptr);         // start zero end null for root node
    activepoint = new ActivePoint(root); // active point starts at root
    globalEnd = new End(-1);             // global end starts at -1 because only root is available and it increases with phase
    currentStringID = -1;                // initially no string is being processed
}

Suffix::~Suffix() {}

// --- End Class Implementation ---
Suffix::End::End(int end)
{
    this->end = end;
}

Suffix::End::~End() {}

// --- Node Class Implementation ---
Suffix::Node::Node(int i, End* e)
{
    suffixLink = nullptr;
    index = -1; // -1 indicates it's not a leaf node
    start = i;  // start index of edge
    end = e;    // end of edge, global end for leaf node
    leafCount = 0; // number of leaves under this node
    for (int k = 0; k < 6; k++)
    { // initialize all children to null
        child[k] = nullptr;
    }
}

Suffix::Node::~Node() {}

// --- ActivePoint Class Implementation ---
Suffix::ActivePoint::ActivePoint(Node* node)
{
    activeNode = node; // starts at root
    activeEdge = -1;   // -1 indicates no active edge
    activelength = 0;  // active length starts at 0
}

Suffix::ActivePoint::~ActivePoint() {}

// --- Main Algorithm Functions ---

void Suffix::makeSuffixTree(string str)
{ // main function to make suffix tree
    text = str;
    for (int i = 0; i < text.length(); i++)
    {
        startPhase(i);
    }
}

 void Suffix::startPhase(int i)
 {                                  // function per phase/ char
     Node* lastnode = NULL; // to store last created internal node (from previous phase) for suffix link
     globalEnd->end = i;      // increment global end for rule 1 extension for leaves
     remaining++;           // increment remaining suffix count

     while (remaining > 0)
     { // while there are suffixes to be added
         if (activepoint->activelength == 0)
         { // if active length is 0

             if (activepoint->activeNode->child[getIndex(text[i])] != nullptr)
             {
                 // RULE 3 EXTENSION!! SHOWSTOPPER
                 activepoint->activeEdge = i; // set active edge to current char index
                 activepoint->activelength ++; // increment active length
                 if (lastnode != NULL )
                 {                                                                       // if there is a last created internal node
                     lastnode->suffixLink = activepoint->activeNode; // connect suffix link to current active node
                     lastnode = NULL;        // remove last node since suffix link is already created
                 }
                 break;

             }
             else
             {
                 // RULE 2 EXTENSION
                 Node* node = new Node(i, globalEnd);                                          // create new leaf node
                 node->index = i - remaining + 1;                                                // set index to current string ID
                 activepoint->activeNode->child[getIndex(text[i])] = node; // add it to active node's children
                 remaining--;                                                                  // decrement remaining suffix count

                 if (lastnode != NULL)
                 {                                                                       // if there is a last created internal node
                     lastnode->suffixLink = activepoint->activeNode; // connect suffix link to current active node
                     lastnode = NULL;                                        // remove last node since suffix link is already created
                 }
                 if (activepoint->activeNode != root)

                 { // if active node is not root
                     if (activepoint->activeNode->suffixLink != nullptr) {  //uiug
                         activepoint->activeNode = activepoint->activeNode->suffixLink;
                     }
                     else activepoint->activeNode = root;
			activepoint->activeEdge = i - remaining + 1;
                     // follow suffix link, where active node becomes the suffix linked node of current node
                 }
                 else {
                     activepoint->activeEdge = i - remaining + 1; // set active edge to next suffix's first character
                     if (activepoint->activelength > 0) activepoint->activelength--;                  // decrement active length
                 }
             }
         }
         else
         { // if active length is NOT 0

             int currentedge = getIndex(text[activepoint->activeEdge]);   // get index of active edge character
             Node* oldnode = activepoint->activeNode->child[currentedge]; // node of current active edge

             // WALKDOWN
             int edgeLen = oldnode->end->end - oldnode->start + 1; // length of current edge (edge len = end_of_node - start_of_node +1)
             if (activepoint->activelength >= edgeLen)
             { // if active length is greater than or equal to edge length
                 // move active point down the tree
                 activepoint->activeEdge += edgeLen;   // move active edge forward by edge length
                 activepoint->activelength -= edgeLen; // decrease active length by edge length
                 activepoint->activeNode = oldnode;    // set active node to node of current active edge
                 continue;
             }

             if (text[oldnode->start + activepoint->activelength] == text[i])
             { // next char on edge matches current char
                 // RULE 3 EXTENSION!! SHOWSTOPPER

                 if (lastnode != NULL)
                 {                                                                       // if there is a last created internal node
                     lastnode->suffixLink = activepoint->activeNode; // make the suffix link of last node be the current active node
                     lastnode = NULL;                                // remove last node since suffix link is already created
                 }

                 activepoint->activelength++; // increment active length
                 break;
             }
             else
             { // next char on edge DOES NOT matche current char

                 // split edge
                 // RULE 2 EXTENSION

                 End* currentend = new End(activepoint->activelength + oldnode->start - 1); // end for current edge after split = active length + start of old node -1
                 Node* splitnot = new Node(oldnode->start, currentend);                      // new internal node created for the split edge
                 Node* newnode = new Node(i, globalEnd);                                     // new leaf node for current character
                 newnode->index = i - remaining + 1;                                           // set index to current string ID
                 activepoint->activeNode->child[currentedge] = splitnot;                     // replace old edge with split node just created
                 oldnode->start += activepoint->activelength;                                // update start of old node to be after split node inserted (currentend + 1)
                 splitnot->child[getIndex(text[i])] = newnode;                               // add new leaf node as child of split internal node just created
                 splitnot->child[getIndex(text[oldnode->start])] = oldnode;                  // reattach the old child node as child of split internal node just created (after editing its start to reflect the end of split node)

                 if (lastnode != NULL)
                 {                                            // if there is a last created internal node
                     lastnode->suffixLink = splitnot; // connect suffix link to the current split node from last internal node created
                 }

                 lastnode = splitnot; // update last created internal node to current split node
                 remaining--;         // decrement suffix count remaining

                 if (activepoint->activeNode == root)
                 {                                        // if active node is root
                     activepoint->activeEdge ++;   // increment active edge
                     activepoint->activelength--; // decrement active length
                 }
                 else {
                     if (activepoint->activeNode->suffixLink != nullptr) activepoint->activeNode = activepoint->activeNode->suffixLink;
                     else activepoint->activeNode = root;
                 }
             }
         }
     }
 }

Suffix::Node* Suffix::getNode(int index)
{
    switch (text[index])
    {
    case 'A':
        return activepoint->activeNode->child[0];
    case 'C':
        return activepoint->activeNode->child[1];
    case 'G':
        return activepoint->activeNode->child[2];
    case 'T':
        return activepoint->activeNode->child[3];
    case '$':
        return activepoint->activeNode->child[4];
    case '#':
        return activepoint->activeNode->child[5];
    }
    return nullptr;
}

int Suffix::getIndex(char c)
{
    switch (c)
    {
    case 'A':
        return 0;
    case 'C':
        return 1;
    case 'G':
        return 2;
    case 'T':
        return 3;
    case '$':
        return 4;
    case '#':
        return 5;
    }
    return -1;
}

//the new lcs with string implemetation
    void Suffix::buildGST(const string concatedText) {
        this->text = concatedText; // concated text

        for (int i = 0; i < (int)text.length(); i++) {  //insert the whole text into suffix tree
            startPhase(i);
        } 
    }

    //THE EDITS THAT I HAVE MADE ARE BELOWWWWWWWWWWWWWWWWWW
    int Suffix::edgeLength(Node* node) {
    // No node or Root node has no incoming edge then length is 0
    if (node == nullptr || node->end == nullptr){
        return 0;
    }
    // Length of edge label = end - start + 1
    return node->end->end - node->start + 1;
    }

    bool Suffix::isLeaf(Node* node) {
    return (node->index != -1); // checks if the index is not -1 to determine if it's a leaf
    }

    void Suffix::dfs_LCS(Node* node, int s1Length, string& concatenatedText, int pathLen, 
             bool& hasSuffixFromS1, bool& hasSuffixFromS2, int& maxLength, 
             string& LCS, string currentPath) {
    
    if (node == nullptr) {
        hasSuffixFromS1 = false;
        hasSuffixFromS2 = false;
        return;
    }
    
    // If this is a leaf node
    if (isLeaf(node)) {
        if (node->index < s1Length) {
            hasSuffixFromS1 = true;
            hasSuffixFromS2 = false;
        } else if (node->index > s1Length) {
            hasSuffixFromS1 = false;
            hasSuffixFromS2 = true;
        } else {
            // Suffix starts at separator '$'
            hasSuffixFromS1 = false;
            hasSuffixFromS2 = false;
        }
        return;
    }
    
    bool foundinS1 = false;
    bool foundinS2 = false;
    
    // Process all children
    for (int i = 0; i < 6; i++) {
        if (node->child[i] == nullptr) {
            continue;
        }
        
        Node* child = node->child[i];
        int edgeLen = edgeLength(child);
        
        // Build the edge string
        string edgeString = concatenatedText.substr(child->start, edgeLen);
        string childPath = currentPath + edgeString;
        
        bool childHasSuffixFromS1 = false;
        bool childHasSuffixFromS2 = false;
        
        dfs_LCS(child, s1Length, concatenatedText, pathLen + edgeLen, 
                childHasSuffixFromS1, childHasSuffixFromS2, maxLength, LCS, childPath);
        
        if (childHasSuffixFromS1) {
            foundinS1 = true;
        }
        if (childHasSuffixFromS2) {
            foundinS2 = true;
        }
    }
    
    // AFTER processing all children, check if this node represents a common substring
    if (foundinS1 && foundinS2) {
        if (pathLen > maxLength) {
            maxLength = pathLen;
            LCS = currentPath;
        }
    }
    
    hasSuffixFromS1 = foundinS1;
    hasSuffixFromS2 = foundinS2;
}

string Suffix::findLargestCommonRegion(const string s1, const string s2) {
    text = s1 + '$' + s2 + '#';
    buildGST(text);
    
    if (root == nullptr || text.empty()) {
        return "";
    }
    
    string LCS = "";
    int maxLength = 0;
    bool hasSuffixFromS1 = false;
    bool hasSuffixFromS2 = false;
    
    dfs_LCS(root, s1.length(), text, 0, hasSuffixFromS1, hasSuffixFromS2, 
            maxLength, LCS, "");
    
    return LCS;
}

int* Suffix::searchPattern(const std::string& pattern, int& count)
{
    Node* current = root;
    int i = 0;

    while (i < pattern.length())
    {
        int idx = getIndex(pattern[i]);
        if (idx == -1 || current->child[idx] == nullptr)
            return nullptr;

        Node* next = current->child[idx];
        int edgeLen = next->end->end - next->start + 1;

        for (int j = 0; j < edgeLen && i < pattern.length(); j++, i++)
        {
            if (text[next->start + j] != pattern[i])
                return nullptr;
        }
        current = next;
    }

    int capacity = 200000;
    int* result = new int[capacity];
    count = 0;

    collectLeafIndices(current, result, count, capacity);
    return result;
}

void Suffix::collectLeafIndices(Node* node, int*& arr, int& count, int& capacity)
{
    if (!node)
        return;

    if (node->index != -1)
    {
        if (count == capacity)
        {
            capacity *= 2;
            int* newArr = new int[capacity];
            for (int i = 0; i < count; i++)
                newArr[i] = arr[i];
            delete[] arr;
            arr = newArr;
        }
        arr[count++] = node->index;
        return;
    }

    for (int i = 0; i < 6; i++)
    {
        collectLeafIndices(node->child[i], arr, count, capacity);
    }
}

int Suffix::countLeaves(Node* node)
{ // function to compute leaf count for each node

    if (node == NULL)
    { // If node is null (aka no node exists)
        return 0;
    }
    if (node->index != -1)
    { // If node is a leaf node (index is -1 when not a leaf/internal node)
        node->leafCount = 1;
        return 1;
    }
    int sum = 0;                        // sum to store total leaf count from all children
    for (int i = 0; i < 6; i++)         // for all possible children per node
        sum += countLeaves(node->child[i]); // recursively compute leaf count for each child and add to sum

    node->leafCount = sum; // set leaf count of current node to sum of leaf counts of all its children
    return sum;
}

void Suffix::findUniqueRegion(Node* node, int x, int currentLength, string* arr, int& index, string& currentPath)
{
    if (!node || index >= 3) //if node is null or the ouput is full 
        return;

    for (int i = 0; i < 6; i++) // for all possible children
    {
        Node* child = node->child[i]; // get child node
        if (!child) // if child node is null, continue to next child
            continue;

        int edgeLen = child->end->end - child->start + 1; //length of current edge (from node to child)
        int newLength = currentLength + edgeLen; // new length after including this edge

        long long oldSize = currentPath.size(); // the size of currentpath is stored before appending the new edge labels
        //so it can be restored when recursing

        currentPath.append(text, child->start, edgeLen); //appends edge label of current child node (path built incrementally)

        if (child->leafCount == 1 &&
            currentLength < x && newLength >= x &&
            index < 3) // if current length is less than x and new length is greater than or equal to x and the output array index is less than 3
        {
            //when leaf count is 1, the substring occurs exactly once and therefore is unique
            //current length and new length conditions just verify that length x is crossed

            // to capture substrings of EXACT length x, which happens when BOTH
            // before adding this edge, the path was SHORTER than x
            // after adding this edge, the path becomes >= x

            //index 3 condition is to store at most 3 results


            arr[index++] = currentPath.substr(0, x); // store the first x characters of the path
        }

        findUniqueRegion(child, x, newLength, arr, index, currentPath);

        currentPath.resize(oldSize); //restores path to previous size
    }
}

void Suffix::findMaxRepetition(Node* node, int x, int& count,
    int currentLength, string& currentPath, string& res)
{
    if (!node) //if node is null, return

        return;

    for (int i = 0; i < 6; i++) // for all possible children
    {
        Node* child = node->child[i]; // get child node
        if (!child) // if child node is null, continue to next child
            continue;

        int edgeLen = child->end->end - child->start + 1; //length of current edge (from node to child)
        int newLength = currentLength + edgeLen; // new length after including this edge

        long long oldSize = currentPath.size(); // the size of currentpath is stored before appending the new edge labels
        //so it can be restored when recursing

        currentPath.append(text, child->start, edgeLen); //appends edge label of current child node (path built incrementally)


        if (currentLength < x && newLength >= x) //current length and new length conditions just verify that length x is crossed
        {
            // to capture substrings of EXACT length x, which happens when BOTH
            // before adding this edge, the path was SHORTER than x
            // after adding this edge, the path becomes >= x

            if (child->leafCount > count)
            {
                // since leafCount is the number of suffixes that share this path
                // the substring stored is what has the largest leaf count of all
                count = child->leafCount;
                res = currentPath.substr(0, x);
            }
        }

        findMaxRepetition(child, x, count, newLength, currentPath, res);
        currentPath.resize(oldSize);
    }
}