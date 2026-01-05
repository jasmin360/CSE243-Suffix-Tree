#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ==========================================
//        SUFFIX TREE CLASS DEFINITION
// ==========================================
class Suffix {
public:
    string text;

    class End {
    public:
        End(int end) { this->end = end; }
        int end;
    };
    End* globalEnd;

    class Node {
    public:
        Node(int i, End* e) {
            start = i; end = e; suffixLink = nullptr; index = -1; leafCount = 0;
            for (int k = 0; k < 6; k++) child[k] = nullptr;
        }
        Node* suffixLink;
        Node* child[6];
        int start;
        End* end;
        int index; // Leaf index
        int leafCount;
    };

    class ActivePoint {
    public:
        ActivePoint(Node* node) {
            activeNode = node; activeEdge = -1; activelength = 0;
        }
        Node* activeNode;
        int activeEdge;
        int activelength;
    };

    Node* root;
    ActivePoint* activepoint;
    int remaining;


    Suffix() {
        remaining = 0;
        globalEnd = new End(-1);
        root = new Node(-1, globalEnd);
        activepoint = new ActivePoint(root);
    }

    // --- Helper: Safe Indexing ---
    int getIndex(char c) {
        switch (c) {
        case 'A': case 'a': return 0;
        case 'C': case 'c': return 1;
        case 'G': case 'g': return 2;
        case 'T': case 't': return 3;
        case '$': return 4;
        case '#': return 5;
        default: return 5;
        }
    }

    Node* getNode(int index) {
        int idx = getIndex(text[index]);
        if (idx >= 0 && idx < 6) return activepoint->activeNode->child[idx];
        return nullptr;
    }

    // --- Build Tree ---
    void makeSuffixTree(string str) {
        text = str;
        remaining = 0;
        if (globalEnd) globalEnd->end = -1;
        for (int i = 0; i < 6; i++) root->child[i] = nullptr;
        cout << "String length: " << text.length() << endl;

        for (int i = 0; i < text.length(); i++) {
            startPhase(i);
        }
    }

    void startPhase(int i)
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
                    activepoint->activelength++; // increment active length
                    if (lastnode != NULL)
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
                        activepoint->activeEdge++;   // increment active edge
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


    // --- SEARCH FUNCTIONALITY ---
    void collectLeafIndices(Node* node, int*& arr, int& count, int& capacity)
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

    int* searchPattern(const std::string& pattern, int& count)
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

        int capacity = 2000;
        int* result = new int[capacity];
        count = 0;

        collectLeafIndices(current, result, count, capacity);
        return result;
    }

    // --- VISUALIZATION ---
    void printTree(Node* node, int depth = 0) {
        if (!node) return;
        for (int i = 0; i < 6; i++) {
            Node* child = node->child[i];
            if (child) {
                for (int j = 0; j < depth; j++) cout << "  |";
                cout << "__";
                int start = child->start;
                int end = *(new int(child->end->end));
                if (end >= text.length()) end = text.length() - 1;

                string edgeStr = "";
                for (int k = start; k <= end; k++) edgeStr += text[k];
                cout << " " << edgeStr;
                if (child->index != -1) cout << " [Leaf Index: " << child->index << "]";
                cout << endl;
                printTree(child, depth + 1);
            }
        }
    }
    int countLeaves(Node* node)
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
    void findUniqueRegion(Node* node, int x, int currentLength,
        string* arr, int& index, string& currentPath)
    {
        if (!node || index >= 3)
            return;

        for (int i = 0; i < 6; i++)
        {
            Node* child = node->child[i];
            if (!child)
                continue;

            int edgeLen = child->end->end - child->start + 1;
            int newLength = currentLength + edgeLen;

            // 🔐 Track path length BEFORE append
            size_t oldSize = currentPath.size();

            // Append edge safely
            currentPath.append(text, child->start, edgeLen);

            if (child->leafCount == 1 &&
                currentLength < x && newLength >= x &&
                index < 3)
            {
                // Extract candidate substring
                string candidate = currentPath.substr(0, x);

                // Reject substrings that include the terminator
                if (candidate.find('$') == string::npos)
                {
                    arr[index++] = candidate;
                }
            }

            findUniqueRegion(child, x, newLength, arr, index, currentPath);

            // 🔐 Restore EXACT previous size (no underflow possible)
            currentPath.resize(oldSize);
        }
    }

    void findMaxRepetition(Node* node, int x, int& count,
        int currentLength, string& currentPath, string& res)
    {
        if (!node)
            return;

        for (int i = 0; i < 6; i++)
        {
            Node* child = node->child[i];
            if (!child)
                continue;

            int edgeLen = child->end->end - child->start + 1;
            int newLength = currentLength + edgeLen;


            size_t oldSize = currentPath.size();

            // Append this edge label
            currentPath.append(text, child->start, edgeLen);

            // Check repetition crossing length x
            if (currentLength < x && newLength >= x)
            {
                if (child->leafCount > count)
                {
                    count = child->leafCount;
                    res = currentPath.substr(0, x);
                }
            }

            // DFS
            findMaxRepetition(child, x, count, newLength, currentPath, res);


            currentPath.resize(oldSize);
        }
    }

    void buildGST(const string concatedText) {
        this->text = concatedText; // concated text

        for (int i = 0; i < (int)text.length(); i++) {  //insert the whole text into suffix tree
            startPhase(i);
        }
    }

    //THE EDITS THAT I HAVE MADE ARE BELOWWWWWWWWWWWWWWWWWW
    int edgeLength(Node* node) {
        // No node or Root node has no incoming edge then length is 0
        if (node == nullptr || node->end == nullptr) {
            return 0;
        }
        // Length of edge label = end - start + 1
        return node->end->end - node->start + 1;
    }

    bool isLeaf(Node* node) {
        return (node->index != -1); // checks if the index is not -1 to determine if it's a leaf
    }

    void dfs_LCS(Node* node, int s1Length, string& concatenatedText, int pathLen,
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
            }
            else if (node->index > s1Length) {
                hasSuffixFromS1 = false;
                hasSuffixFromS2 = true;
            }
            else {
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

    string findLargestCommonRegion(const string s1, const string s2) {
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


};


// ==========================================
//              MAIN FUNCTION
// ==========================================
int main() {
    cout << "--- Suffix Tree Search Debugger ---" << endl;

    // 1. Define Test String (with repeats)
    // "TG" appears at index 0 and 3
    string dnaTest = "CACATGTCATGT$";
        // 2. Build Tree
    string lol = "";
    for (char c : dnaTest) {
        if (c == 'A' || c == 'C' || c == 'G' || c == 'T' || c == '$') {
            lol += c;
        }
    }
    cout << "\n[Building Tree for]: " << lol << endl;
    Suffix tree;
    tree.makeSuffixTree(lol);
    cout << "jsdiajbdihabSd: " << dnaTest.length() << endl;
    tree.printTree(tree.root); 
    string* arr = new string[3];
    tree.countLeaves(tree.root);
    int index = 0;
    string path = "";

    tree.findUniqueRegion(tree.root,5, 0, arr, index, path);

    // OUTPUT
    for (int i = 0; i < index; i++)
    {
        cout << "Unique region " << i + 1 << ": " << arr[i] << endl;
    }


    return 0;
}