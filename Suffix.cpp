#include <iostream>
#include "Suffix.h"
#include <vector>
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
    globalEnd->end++;      // increment global end for rule 1 extension for leaves
    remaining++;           // increment remaining suffix count

    while (remaining > 0)
    { // while there are suffixes to be added
        if (activepoint->activelength == 0)
        { // if active length is 0

            if (getNode(i) != NULL)
            {
                // RULE 3 EXTENSION!! SHOWSTOPPER
                activepoint->activeEdge = i; // set active edge to current char index
                activepoint->activelength++; // increment active length
                break;
            }
            else
            {
                // RULE 2 EXTENSION
                Node* node = new Node(i, globalEnd);                                          // create new leaf node
                node->index = i - remaining +1;                                                // set index to current string ID
                activepoint->activeNode->child[getIndex(text[i])] = node; // add it to active node's children
                remaining--;                                                                  // decrement remaining suffix count

                if (lastnode != NULL)
                {                                                                       // if there is a last created internal node
                    lastnode->suffixLink = activepoint->activeNode; // connect suffix link to current active node
                    lastnode = NULL;                                        // remove last node since suffix link is already created
                }
                if (activepoint->activeNode != root)
                { // if active node is not root
                    activepoint->activeNode = activepoint->activeNode->suffixLink;
                    // follow suffix link, where active node becomes the suffix linked node of current node
                }
            }
        }
        else
        { // if active length is NOT 0

            int currentedge = getIndex(text[activepoint->activeEdge]);   // get index of active edge character
            Node* oldnode = activepoint->activeNode->child[currentedge]; // node of current active edge

            // If the child edge does not exist under the current active node,
            // it's effectively a rule 2 extension: create a new leaf.
            if (oldnode == nullptr)
            {
                Node* node = new Node(i, globalEnd);
                node->index = i - remaining + 1;
                activepoint->activeNode->child[currentedge] = node;
                remaining--;

                if (lastnode != NULL)
                {
                    lastnode->suffixLink = activepoint->activeNode;
                    lastnode = NULL;
                }

                if (activepoint->activeNode != root)
                {
                    activepoint->activeNode = activepoint->activeNode->suffixLink;
                }

                // continue to next suffix to insert
                continue;
            }

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
                    lastnode = NULL;                                        // remove last node since suffix link is already created
                }

                activepoint->activelength++; // increment active length
                break;                       // SHOWSTOPPERR
            }
            else
            { // next char on edge DOES NOT matche current char

                // split edge
                // RULE 2 EXTENSION

                End* currentend = new End(activepoint->activelength + oldnode->start - 1); // end for current edge after split = active length + start of old node -1
                Node* splitnot = new Node(oldnode->start, currentend);                      // new internal node created for the split edge
                Node* newnode = new Node(i, globalEnd);                                     // new leaf node for current character
                newnode->index = i - remaining +1;                                           // set index to current string ID
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
                    activepoint->activeEdge++;   // increment active edge
                    activepoint->activelength--; // decrement active length
                }
                else
                    activepoint->activeNode = activepoint->activeNode->suffixLink; // if active node is NOT root, follow suffix link
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


    
    void Suffix::buildGST(const char* s1, const char* s2) {
        this->text = ""; // text before concat

        // Append s1
        for (int i = 0; s1[i] != '\0'; i++){
            this->text += s1[i];
        }
        int s1Length = this->text.length(); //store length of s1 for later use
        this->text += '$';

        // Append s2
        for (int i = 0; s2[i] != '\0'; i++){
            this->text += s2[i];
        }   
        this->text += '#';

        // textexample = "s1$s2#"
        int pos = 0;
        // inserting chars before the '$'
        this->currentStringID = 0;
        while (this->text[pos] != '$') {
            this->startPhase(pos);
            pos++;
        }
        this->startPhase(pos); // insert '$'
        pos++;
        // inserting chars before the '#'
        this->currentStringID = 1;
        while (this->text[pos] != '#') {
            this->startPhase(pos);
            pos++;
        }
        this->startPhase(pos); // insert '#'

        
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

    void Suffix::dfs_LCS(Node* node, int s1Length, char* tempPath, int pathLen, bool& hasSuffixFromS1, bool& hasSuffixFromS2, int& maxLength, char* LCS){
        /*Guide for variables
        s1Length= length of s1 without the $
        tempPath= contains temporary path for characters during the dfs
        pathLen= length of current path
        hasSuffixFromS1= boolean to check if suffix from s1 is found in current path
        hasSuffixFromS2= boolean to check if suffix from s2 is found in current path
        maxLength= length of longest common suffix found so far
        LCS= character array to store longest common suffix found so far
        */

        if (node == nullptr) { //saftey ] stops recursion
            hasSuffixFromS1 = false;
            hasSuffixFromS2 = false;
            return;
        }

        if(isLeaf(node)){ // checks if the node is leaf using method isLeaf
            int suffixStart = node -> index; // determine suffix start possition using index
            if(suffixStart < s1Length){ // if suffix start < length of s1 it belongs to s1
                hasSuffixFromS1 = true;
                hasSuffixFromS2 = false;
            }
            else if (suffixStart > s1Length) { // else it belongs to s2
                hasSuffixFromS1 = false;
                hasSuffixFromS2 = true;
            }
            else{ // it means its exactly a the $ so we ignore
                hasSuffixFromS1 = false;
                hasSuffixFromS2 = false;
            }
            return;
        }

        bool foundinS1 = false;  //it will turn true if any child subtree has suffix from s1
        bool foundinS2 = false; //it will turn true if any child subtree has suffix from s2

        // loop over all the possible children (6 for A,C,G,T,$,#)
        for(int i=0; i<6; i++){
            Node* childPointer = node->child[i]; // get the child pointer
            
            if(childPointer == nullptr){ // if child is null continue to next child
                continue;
            }

            int len = edgeLength(childPointer); // get edge length using method edgeLength

            int oldPathLen = pathLen; // store old path length

            for(int m=0; m<len; m++){ // walk through edge and add characters to tempPath
                char c = text[childPointer->start + m]; 

                if(c== '$' || c=='#'){ // ignore special characters
                    break;
                }
                tempPath[pathLen] = c; // add character to tempPath
                pathLen++;  //increase path length
            }

            bool childHasSuffixFromS1 = false; // to check if child subtree has suffix from s1
            bool childHasSuffixFromS2 = false; // to check if child subtree has suffix from s2

            dfs_LCS(childPointer, s1Length, tempPath, pathLen, childHasSuffixFromS1, childHasSuffixFromS2, maxLength, LCS); // recursive DFS call

            /*bAsically if any of the children contains a suffix from s1 or s2 this will mean that that this node has suffix from s1 or s2. 
            A node represents a substring that appears in string 2 when at least one of its children represents a suffix appearing in string 2 for example
            so using this same example if one child has string 2, the parent as well has string 2
            if we think about it conceptually the parent inherits the presence of a string from its children and one child is enough*/

            if(childHasSuffixFromS1){
                foundinS1 = true; //if any child has suffix from s1 mark foundinS1 true
            }
            if(childHasSuffixFromS2){
                foundinS2 = true; //if any child has suffix from s2 mark foundinS2 true
            }

            // if the current node subtree has the suffixes from both the strings then its common
            if(foundinS1 && foundinS2){ // common substring condition
                    // check if the common substring is longer than the maxLength found so far
                    if(pathLen > maxLength){
                        maxLength = pathLen;
                        for(int l=0; l<pathLen; l++){
                            LCS[l] = tempPath[l]; // update LCS with the current path
                        }
                        LCS[pathLen] = '\0'; // null terminate the LCS string
                    }
            }

            pathLen = oldPathLen; // backtrack path length to old value
        }

            hasSuffixFromS1 = foundinS1; // update hasSuffixFromS1 for parent node
            hasSuffixFromS2 = foundinS2; // update hasSuffixFromS2 for parent node
    }    

    string Suffix::findLargestCommonRegion(const char* s1, const char* s2){

        this->buildGST(s1, s2);
        int s1Length = strlen(s1); //length of s1 for later use
        // If tree not built or text empty
        if (this->root == nullptr || this->text.empty()) {
            return ""; // empty result
        }

        char* tempPath = new char[this->text.size() + 1]; // temporary array to store LCS
        char* LCS = new char[this->text.size() + 1]; // array to store final LCS
        LCS[0] = '\0'; // start empty

        int maxLength = 0; // to store maximum length of common substring
        bool hasSuffixFromS1 = false; // to check if suffix from s1 is found
        bool hasSuffixFromS2 = false; // to check if suffix from s2 is

        dfs_LCS(root, s1Length, tempPath, 0, hasSuffixFromS1, hasSuffixFromS2, maxLength, LCS);

        string result = string(LCS);
        delete[] tempPath;
        delete[] LCS;

        return result;
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

    int capacity = 20000;
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

void Suffix::findUniqueRegion(Node* node, int x ,int currentLength, string* arr, int index)
{ 
    if (!node)
    { // if node is null
        return;
    }

    for (int i = 0; i < 6; i++)
    { // for all possible children
        Node* child = node->child[i]; // get child node
        if (!child)
        { // if child node is null, continue
            continue;
        }

        int edgeLen = child->end->end - child->start + 1; // length of current edge
        int newLength = currentLength + edgeLen;          // new length after including this edge

        if (child->leafCount == 1)
        { // if current subtree occurs exactly once (unique region)
            if (currentLength < x && newLength >= x)
            { // if current length is less than x and new length is greater than or equal to x
                // extract substring of length x
                int startIndex = child->end->end - (newLength - x) + 1;
                arr[index] = text.substr(startIndex, x);
            }
        }

        findUniqueRegion(child, newLength, x, arr, index); // recursive depth first search
    }
}