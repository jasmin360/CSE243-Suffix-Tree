#include <iostream>
#include "Suffix.h"
#include <vector>
using namespace std;

class Suffix
{
public:
    Suffix()
    {
        remaining = 0;
        root = new Node(0, nullptr);         // start zero end null for root node
        activepoint = new ActivePoint(root); // active point starts at root
        globalEnd = new End(-1);             // global end starts at -1 because only root is available and it increases with phase
        currentStringID = -1;                // initially no string is being processed
    };
    ~Suffix();
    class End
    {
    public:
        End(int end)
        {
            this->end = end;
        };
        ~End();
        int end;
    };
    End *globalEnd; // global end pointer for all leaf nodes
    class Node
    {
    public:
        Node(int i, End *e)
        {
            suffixLink = nullptr;
            index = -1; //-1 indicates it's not a leaf node
            start = i;  // start index of edge
            end = e;    // end of edge, global end for leaf node
            leafCount = 0; //number of leaves under this node
            for (int k = 0; k < 6; k++)
            { // initialize all children to null
                child[k] = nullptr;
            }
        }

        Node *child[6]; // array of 6 chars for all possible characters A,C,G,T,$,#
        ~Node();
        Node *suffixLink;
        int index;
        End *end;
        int start;
        int leafCount;
    };

    class ActivePoint
    {
    public:
        ActivePoint(Node *node)
        {
            activeNode = node; // starts at root
            activeEdge = -1;   //-1 indicates no active edge
            activelength = 0;  // active length starts at 0
        };
        ~ActivePoint();
        Node *activeNode;
        int activeEdge;
        int activelength;
    };
    Node *root; // start node
    string text;
    ActivePoint *activepoint;
    int currentStringID; // To determine the ownership of each suffix
    int remaining;

    void Suffix::makeSuffixTree(string str)
    { // main function to make suffix tree
        text = str;
        for (int i = 0; i < text.length(); i++)
        {
            startPhase(i);
        }
    };

    void startPhase(int i)
    {                          // function per phase/ char
        Node *lastnode = NULL; // to store last created internal node (from previous phase) for suffix link
        globalEnd->end++;      // increment global end for rule 1 extension for leaves
        remaining++;           // increment remaining suffix count

        while (remaining > 0)
        { // while there are suffixes to be added
            if (activepoint->activelength == 0)
            { // if active length is 0

                if (getNode(i) != NULL)
                { // if there is an edge starting with current char
                    // RULE 3 EXTENSION!! SHOWSTOPPER
                    activepoint->activeEdge = i; // set active edge to current char index
                    activepoint->activelength++; // increment active length
                    break;
                }
                else
                { // if NO edge starting with current char exists
                    // RULE 2 EXTENSION
                    Node *node = new Node(i, globalEnd);                      // create new leaf node
                    node->index = currentStringID;                            // set index to current string ID
                    activepoint->activeNode->child[getIndex(text[i])] = node; // add it to active node's children
                    remaining--;                                              // decrement remaining suffix count

                    if (lastnode != NULL)
                    {                                                   // if there is a last created internal node
                        lastnode->suffixLink = activepoint->activeNode; // connect suffix link to current active node
                        lastnode = NULL;                                // remove last node since suffix link is already created
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
                Node *oldnode = activepoint->activeNode->child[currentedge]; // node of current active edge

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
                    {                                                   // if there is a last created internal node
                        lastnode->suffixLink = activepoint->activeNode; // make the suffix link of last node be the current active node
                        lastnode = NULL;                                // remove last node since suffix link is already created
                    }

                    activepoint->activelength++; // increment active length
                    break;                       // SHOWSTOPPERR
                }
                else
                { // next char on edge DOES NOT matche current char

                    // split edge
                    // RULE 2 EXTENSION

                    End *currentend = new End(activepoint->activelength + oldnode->start - 1); // end for current edge after split = active length + start of old node -1
                    Node *splitnot = new Node(oldnode->start, currentend);                     // new internal node created for the split edge
                    Node *newnode = new Node(i, globalEnd);                                    // new leaf node for current character
                    newnode->index = currentStringID;                                          // set index to current string ID
                    activepoint->activeNode->child[currentedge] = splitnot;                    // replace old edge with split node just created
                    oldnode->start += activepoint->activelength;                               // update start of old node to be after split node inserted (currentend + 1)
                    splitnot->child[getIndex(text[i])] = newnode;                              // add new leaf node as child of split internal node just created
                    splitnot->child[getIndex(text[oldnode->start])] = oldnode;                 // reattach the old child node as child of split internal node just created (after editing its start to reflect the end of split node)

                    if (lastnode != NULL)
                    {                                    // if there is a last created internal node
                        lastnode->suffixLink = splitnot; // connect suffix link to the current split node from last internal node created
                    }

                    lastnode = splitnot; // update last created internal node to current split node
                    remaining--;         // decrement suffix count remaining

                    if (activepoint->activeNode == root)
                    {                                // if active node is root
                        activepoint->activeEdge++;   // increment active edge
                        activepoint->activelength--; // decrement active length
                    }
                    else
                        activepoint->activeNode = activepoint->activeNode->suffixLink; // if active node is NOT root, follow suffix link
                }
            }
        }
    };
    Node *getNode(int index)
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
    };

    int getIndex(char c)
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
    };

    void buildGST(const char *s1, const char *s2)
    {
        Suffix gst;    // suffix Tree object
        gst.text = ""; // text before concat

        // Append s1
        for (int i = 0; s1[i] != '\0'; i++)
        {
            gst.text += s1[i];
        }
        gst.text += '$';

        // Append s2
        for (int i = 0; s2[i] != '\0'; i++)
        {
            gst.text += s2[i];
        }
        gst.text += '#';

        // textexample = "s1$s2#"
        int pos = 0;
        // inserting chars before the '$'
        gst.currentStringID = 0;
        while (gst.text[pos] != '$')
        {
            gst.startPhase(pos);
            pos++;
        }
        gst.startPhase(pos); // insert '$'
        pos++;
        // inserting chars before the '#'
        gst.currentStringID = 1;
        while (gst.text[pos] != '#')
        {
            gst.startPhase(pos);
            pos++;
        }
        gst.startPhase(pos); // insert '#'
    }
    int* Suffix::searchPattern(const std::string& pattern, int& count) {
        Node* current = root;
        int i = 0;

        while (i < pattern.length()) {
            int idx = getIndex(pattern[i]);
            if (idx == -1 || current->child[idx] == nullptr)
                return nullptr;

            Node* next = current->child[idx];
            int edgeLen = next->end->end - next->start + 1;

            for (int j = 0; j < edgeLen && i < pattern.length(); j++, i++) {
                if (text[next->start + j] != pattern[i])
                    return nullptr;
            }
            current = next;
        }

        
        int capacity = 10;
        int* result = new int[capacity];
        count = 0;

        collectLeafIndices(current, result, count, capacity);
        return result;
    }


    void Suffix::collectLeafIndices(
        Node* node,
        int*& arr,
        int& count,
        int& capacity
    ) {
        if (!node) return;

        if (node->index != -1) {
            if (count == capacity) {
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

        for (int i = 0; i < 6; i++) {
            collectLeafIndices(node->child[i], arr, count, capacity);
        }
    }

    int countLeaves(Node *node){ // function to compute leaf count for each node

        if (node==NULL){ // If node is null (aka no node exists)
            return 0;
        } 
        if (node->index != -1){ // If node is a leaf node (index is -1 when not a leaf/internal node)
            node->leafCount = 1; 
            return 1;
        }
        int sum = 0; // sum to store total leaf count from all children
        for (int i = 0; i < 6; i++) // for all possible children per node
            sum += countLeaves(node->child[i]); // recursively compute leaf count for each child and add to sum

        node->leafCount = sum; // set leaf count of current node to sum of leaf counts of all its children
        return sum;
    }

    void findUniqueRegion(Node *node, int currentLength, int x, string &result){ // blip.a will do the dynamic array thing in a bit please dont edit it 

        if (!node) { //if node is null
            return;
        }

        for (int i = 0; i < 6; i++){ // for all possible children
            Node *child = node->child[i]; // get child node
            if (!child){ // if child node is null, continue
                continue;
            }

            int edgeLen = child->end->end - child->start + 1; //length of current edge 
            int newLength = currentLength + edgeLen; // new length after including this edge

            if (child->leafCount == 1){ // if current subtree occurs exactly once (unique region)
                if (currentLength < x && newLength >= x){ // if current length is less than x and new length is greater than or equal to x
                    // extract substring of length x
                    int startIndex = child->end->end - (newLength - x) + 1; 
                    result = text.substr(startIndex, x);
                }
            }

        findUniqueRegion(child, newLength, x, result); // recursive depth first search
    }
}