//ukonens algorithm
#pragma once
#include <iostream>
#include <vector>
using namespace std;
class Suffix{
public:
    Suffix();
    ~Suffix();
    
    class End{
    public:
        End(int end);
        ~End();
        int end;
    };
    End* globalEnd;
    class Node{
    public:
        Node(int i, End* e);
        ~Node();
        Node* suffixLink;
        Node* child[6];
        int index;
        End* end;
        int start;
		int leafCount;
    };

    void makeSuffixTree(string str);
    void startPhase(int i);
    int* searchPattern(const string &pattern, int& count);
    Node* getNode(int index);
    int getIndex(char c);
    //the new lcs with string implemetation

    void buildGST(const string concatedText);
    int edgeLength(Node* node);
    bool isLeaf(Node* node);
    int countLeaves(Node* node);
    string findLargestCommonRegion(const char* s1, const char* s2);
    void dfs_LCS(Node* node, int s1Length, string& concatenatedText, int pathLen, 
             bool& hasSuffixFromS1, bool& hasSuffixFromS2, int& maxLength, 
             string& LCS, string currentPath);
    string findLargestCommonRegion(const string s1, const string s2);
    void findUniqueRegion(Node* node, int x, int currentLength, string* arr, int& index);
    void findMaxRepetition(Node* node, int x, int& count, int currentLength, string& res);
    class ActivePoint{
    public:
        ActivePoint(Node* node);
        ~ActivePoint();
        Node* activeNode;
        int activeEdge;
        int activelength;
    };
    Node* root;
    string text;
    ActivePoint* activepoint; 
    int remaining;
	int currentStringID;
    void insert(int index);
    void extend(int index);

    private:
		void collectLeafIndices(Node* node, int*& arr, int& count, int& capacity);

    

};