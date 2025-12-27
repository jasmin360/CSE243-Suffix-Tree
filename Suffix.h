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
    void buildGST(const char* s1, const char* s2);
    int countLeaves(Node* node);
    void findUniqueRegion(Node* node, int currentLength, int x, string& result);
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